#include "buffer.h"

#include <zmq.hpp>

#include <iostream>
#include <thread>         
#include <chrono>         
//#include <sstream>
#include <vector>

#define _RECEIVER_ADDR_ 					"tcp://*:4242"
#define _SENDER_ADDR_							"tcp://*:4343"

#define	_SERIALIZED_BUFFER_PATH_	"./serialized_buffer.txt"


typedef char							buffer_type;

void receiver_thread(zmq::context_t* context, st::shared_buffer<buffer_type>* shared_buff, bool* exit_thread)
{
	// Socket to receive messages on
	zmq::socket_t receiver(*context, ZMQ_PULL);
	try
	{
		receiver.bind(_RECEIVER_ADDR_);
	}
	catch(zmq::error_t& e)
	{
		std::cerr << "failed to bind to endpoint [" << _RECEIVER_ADDR_ << "]. Error: " << e.what() << std::endl;
		exit(1);
	}
		
	try
	{
		// Process tasks forever
		while(!*exit_thread)
		{
			zmq::message_t message;
			receiver.recv(&message);
			
			const size_t msg_size = message.size();
			std::vector<char> buffer(msg_size);
			bool part_message = true;
			
			memcpy((void *)&buffer[0], message.data (), msg_size);
	
			if(!message.more())
			{
				part_message = false;
			}
	
			shared_buff->push(buffer, part_message);
			//std::stringstream print_buffer(static_cast<char*>(message.data()));
			//std::cout << print_buffer.str() << std::endl;
		}
	}
	catch(...)
	{
		std::cerr << "Exiting receiver thread..." << std::endl;
	}
}

void sender_thread(zmq::context_t* context, st::shared_buffer<buffer_type>* shared_buff, bool* exit_thread)
{
	// Socket to send messages to
	zmq::socket_t sender(*context, ZMQ_PUSH);
	try
	{
		sender.bind(_SENDER_ADDR_);
	}
	catch(zmq::error_t& e)
	{
		std::cerr << "failed to bind to endpoint [" << _SENDER_ADDR_ << "]. Error: " << e.what() << std::endl;
		exit(1);
	}
	
	std::vector<char> buffer;
	bool part_message;
	
	try
	{
		while(!*exit_thread)
		{		 	
			if(shared_buff->pop(buffer, part_message))
			{
				//std::cout << std::endl << "Sending message: ";
	
				//std::stringstream print_buffer(static_cast<char*>((*reply).data()));
				//std::cout << "\n sender_thread - sending: " << print_buffer.str() << std::endl;
				
				zmq::message_t reply (buffer.size());
				memcpy((void *) reply.data (), &buffer[0], buffer.size());
	
				sender.send(reply, part_message ? ZMQ_SNDMORE : 0);
			}
			else
			{
				//std::cout << "Sleeping 1 sec. Empty queue..." << std::endl;
				//std::this_thread::sleep_for (std::chrono::milliseconds(100));
			}
		}
	}
	catch(...)
	{
		shared_buff->push(buffer, part_message);
		std::cerr << "Exiting sender thread..." << std::endl;
	}
}

int main (int argc, char *argv[])
{
	zmq::context_t context(1);
	st::shared_buffer<buffer_type> shared_buff;
	bool exit_thread = false;
		
	shared_buff.deserialize(_SERIALIZED_BUFFER_PATH_);
	
	try
	{
		std::thread receiver(receiver_thread, &context, &shared_buff, &exit_thread); 
		std::thread sender(sender_thread, &context, &shared_buff, &exit_thread); 
		//receiver.detach();
		//sender.detach();
	
		std::cout << std::endl << "Write something to terminate the program:" << std::endl;
		std::string dummy;
		std::cin >> dummy;
		exit_thread = true;
		std::cout << std::endl << "Exiting..." << std::endl;
		
		shared_buff.notify_exit();
		context.close();
						
		receiver.join();
		sender.join();	
	}
	catch(...)
	{
		std::cerr << "main exception!" << std::endl;
	}
	
	shared_buff.serialize(_SERIALIZED_BUFFER_PATH_);
		
	return 0;
}