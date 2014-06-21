#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <utility>

#include <thread>
#include <mutex>
#include <condition_variable>

namespace st
{
	template<typename T> class shared_buffer
	{
        std::list<std::pair<std::vector<T>, bool> > internal_buffer;
        std::mutex mutex_;
        std::condition_variable cond_;
        bool exit_thread;
	public:
        shared_buffer() : exit_thread(false) {}
        void push(std::vector<T>& buffer, bool part_message);
        void push_front(std::vector<T>& buffer, bool part_message);
        bool pop(std::vector<T>& buffer, bool& part_message);
            
        bool serialize(const std::string& path);
        bool deserialize(const std::string& path);	
            
        void notify_exit() {
            exit_thread = true;
            cond_.notify_one();
        }				
	};
	
	template <typename T> void shared_buffer<T>::push(std::vector<T>& buffer, bool part_message)
	{
		std::unique_lock<std::mutex> mlock(mutex_);
        internal_buffer.push_back(std::pair<std::vector<T>, bool>(std::move(buffer), part_message));
        mlock.unlock();
        cond_.notify_one();
	}
	
	template <typename T> void shared_buffer<T>::push_front(std::vector<T>& buffer, bool part_message)
	{
		std::unique_lock<std::mutex> mlock(mutex_);
        internal_buffer.push_front(std::pair<std::vector<T>, bool>(std::move(buffer), part_message));
        mlock.unlock();
        cond_.notify_one();
	}

	template <typename T> bool shared_buffer<T>::pop(std::vector<T>& buffer, bool& part_message)
	{
		std::unique_lock<std::mutex> mlock(mutex_);
        while (internal_buffer.empty() && !exit_thread)
        {
            cond_.wait(mlock);
        }
        
        if(exit_thread)
        {
            return false;
        }
		
		std::pair<std::vector<T>, bool> pair = internal_buffer.front();
		buffer = std::move(pair.first);
		part_message = pair.second;
		internal_buffer.pop_front();
		
		return true;
	}
	
	template <typename T> bool shared_buffer<T>::serialize(const std::string& path)
	{
		bool ret_val = true;
		
		std::unique_lock<std::mutex> mlock(mutex_);
			
		if(internal_buffer.empty())
		{
			return ret_val;
		}

        std::ofstream outfile (path,std::ofstream::binary);
  	
        try
        {
            if(!outfile.is_open())
            {
                std::cout << std::endl << "Impossible to open output file: " << path << std::endl;
                return false;
            }
            std::cout << std::endl << "start serializing buffer on path: " << path << std::endl;
            
            while(!internal_buffer.empty())
            {
                std::pair<std::vector<T>, bool> pair = internal_buffer.front();
                std::vector<T> buffer = pair.first;
                bool part_message = pair.second;
                internal_buffer.pop_front();
                
                //std::cout << part_message << ";" << buffer.size() << "\n";
                //std::cout.write(&buffer[0], buffer.size());
                    
                outfile << part_message << ";" << buffer.size() << "\n";
                outfile.write(&buffer[0], buffer.size());
                outfile << part_message << "\n";
            }
        }
        catch(...)
        {
            ret_val = false;
        }
  	
        outfile.close();
        
        std::cout << std::endl << "end serializing buffer on path: " << path << std::endl;
        
        return ret_val;
	}
	
	template <typename T> bool shared_buffer<T>::deserialize(const std::string& path)
	{
		bool ret_val = true;	
			
		std::fstream infile (path, std::ios::in);
		try
        {
            if(!infile.is_open())
            {
                //std::cout << std::endl << "Impossible to open input file: " << path << std::endl;
                return false;
            }
            std::cout << std::endl << "deserializing buffer on path: " << path << std::endl;
                
            std::string line;
            while (getline(infile, line))
            { 	
                std::string string_part = (line.substr(0, line.find_last_of(';')));
                std::string string_message_size = (line.substr(line.find_last_of(';') + 1, line.length()));
                const size_t buffer_size = atoi(string_message_size.c_str());

                std::vector<T> buffer(buffer_size);
                bool part_message = atoi(string_part.c_str());

                infile.read(&buffer[0], buffer_size);
                internal_buffer.push_back(std::pair<std::vector<T>, bool>(std::move(buffer), part_message));
            }
          
            //clear content
            infile.close();
            infile.open( path, std::ios::out | std::ios::trunc );
        }
        catch(...)
        {
            ret_val = false;
        }
  	
        infile.close();
  	
        return ret_val;
	}
}
#endif //_BUFFER_H_
