#include "buffer.h"

#include <zmq.hpp>

#include <iostream>
#include <thread>         
#include <chrono>         
//#include <sstream>
#include <vector>

#define _RECEIVER_ADDR_ 					"tcp://*:4242"
#define _SENDER_ADDR_						"tcp://*:4343"

#define	_SERIALIZED_BUFFER_PATH_            "./serialized_buffer.txt"


typedef char	buffer_type;

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
			st::buffer_obj buffer;
			buffer.msg_size = msg_size;
			buffer.msg = (char*) malloc(msg_size);
			buffer.part_message = false;
			memcpy((void *)&buffer.msg[0], message.data (), msg_size);
	
			if(message.more())
			{
				/*receiver.recv(&message);
				size_t new_msg_size = message.size();
				buffer.msg = (char*) realloc(buffer.msg, (buffer.msg_size + new_msg_size));
				memcpy((void *)&buffer.msg[buffer.msg_size], message.data (), new_msg_size);	
				buffer.msg_size += new_msg_size;*/
				buffer.part_message = true;
			}
	
			shared_buff->push(buffer);

			//std::stringstream print_buffer(static_cast<char*>(message.data()));
			//std::cout << "\n receiver_thread - storing: " << buffer.msg << std::endl;
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
	
	try
	{
		while(!*exit_thread)
		{	
			st::buffer_obj	buffer; 	
			if(shared_buff->pop(buffer))
			{
				//std::cout << std::endl << "Sending message: ";
	
				//std::stringstream print_buffer(static_cast<char*>((*reply).data()));
				//std::cout << "\n sender_thread - sending: " << buffer.msg << std::endl;
				
				zmq::message_t reply (buffer.msg_size);
				memcpy((void *) reply.data (), buffer.msg, buffer.msg_size);
				sender.send(reply, buffer.part_message ? ZMQ_SNDMORE : 0);
				free(buffer.msg);
			}
			else
			{
				//std::this_thread::sleep_for (std::chrono::milliseconds(100));
			}
		}
	}
	catch(...)
	{
		//shared_buff->push_front(buffer, part_message);
		std::cerr << "Exiting sender thread..." << std::endl;
	}
}

int main (int argc, char *argv[])
{
	zmq::context_t context(1);
	st::shared_buffer<buffer_type> shared_buff;
	bool exit_thread = false;
		
	//shared_buff.deserialize(_SERIALIZED_BUFFER_PATH_);
	
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
	
	//shared_buff.serialize(_SERIALIZED_BUFFER_PATH_);
		
	return 0;
}
