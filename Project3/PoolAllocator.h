#pragma once
#include <vector>
#include <sstream>
#include <string>
#include "Logger.h"
#include "data.h"

#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Camera.h"
#include "Bones.h"
#include "Skeleton.h"
#include "Shader.h"
#include "MeshRenderer.h"
#include "SkinnedMeshRenderer.h"
#include "AnimationController.h"

template <class T> //when using templates, we cannot do implementation in a .cpp file, it must be in the header file
class PoolAllocator {
public:

	union Memory_Block { // this lets us represent the same block of memory as two different types
		T data;					// when get_memory() is called, we stop looking at the Memory_Block as a pointer to another Memory_Block, but instead as an Object of type T
		Memory_Block* next;		// when the Memory_Block is free, it is a pointer to the next free Memory_Block
	};

	int size;
	std::vector<T*> used_list;
	std::vector<Memory_Block*> memory; // a pointer to the start of the allocated memory
	Memory_Block* next_free_block; // a pointer to the start of the linked-list of free spaces

	/**
	 * Constructor
	 */
	PoolAllocator(unsigned int size) :
		next_free_block(nullptr),
		size(size)
	{
		add_block();
		Logger::log("PoolAllocator Loaded");
	}

	/**
	 * 
	 */
	~PoolAllocator() {
		Logger::log("PoolAllocator Destruction");
		/*
		for(Memory_Block* block : memory) {
			std::stringstream ss;
			ss << block;
			Logger::log("Free at Address: " + ss.str());
			std::free(block);	//frees all the memory when the PoolAllocator goes out of scope
		}
		memory.empty();
		*/
	}

	void init() {
		if (memory.empty()) {
			add_block();
		}
	}

	/**
	 * Mallocs a chunk of data with size = this.size. Sets the next_free_block to be the first Memory_Block. Each Memory_Block is a pointer to the next.
	 */
	void add_block() {
		memory.push_back((Memory_Block*)std::malloc(sizeof(Memory_Block) * size));	//mallocs a block of data and adds a pointer to in into the memory vector
		std::stringstream ss;
		ss << memory.back();
		//Logger::log("Malloc at address: " + ss.str());
		next_free_block = memory.back();	//the next_free_block now points to the start of this block
		for (int i = 0; i < size - 1; i++) {
			next_free_block[i].next = &next_free_block[i + 1];	// we go through every Memory_Block assigning its value to be a pointer to the next Memory_Block
		}
		next_free_block[size - 1].next = NULL; // the last block points to nothing
	}

	/**
	 * increases this.size before calling add_block(). Normally called whenever the allocator is full
	 */
	void expand() {
		size = size * 2;	// we will malloc twice the size of the previous block
		//Logger::log(std::string("Expanding PoolAllocator<") + std::type_index(typeid(T)).name() + "> by mallocing: " + std::to_string(size));
		add_block();
	}

	T* add(const T& object) {
		return new (get_memory()) T(object);
	}

	T* add() {
		return new (get_memory()) T();
	}

	/**
	 * Request heap memory
	 * @return Memory_Block* pointer to a free block of memory
	 */
	T* get_memory() {
		Memory_Block* free_memory = next_free_block;	// we grab the next free Memory_Block to be given away
		if (free_memory == NULL) {							// if there isn't a free memory block, make more space
			expand();
			free_memory = next_free_block;
		}
		next_free_block = next_free_block->next;		// the next_free_block is now the next in the linked list of free pointers
		used_list.push_back(&(free_memory->data));			// add a pointer to the data into the list of used data
		return &(free_memory->data);					// we return a pointer to the data representation of the Memory_Block (this is the reason why using a union is important as we cannot return a pointer to a Memory_Block. We have to return a pointer to type T)
	}

	/**
	 * Free up the memory previously requested
	 * WARNING don't free up memory that was not returned in get_memory()!!!
	 */
	void free(T* pointer) {					// free a used Memory_Block by adding it to the front of the linked list of free Memory_Blocks
		Memory_Block* b_pointer = (Memory_Block*)pointer;
		remove_item(pointer);			//remove the pointer from the used_data vector
		b_pointer->next = next_free_block;	
		next_free_block = b_pointer;
	}

	void remove_item(T* item_pointer)
	{
		used_list.erase(remove(used_list.begin(), used_list.end(), item_pointer), used_list.end());
	}
};