#pragma once

#include <string>
#include <vector>

#include <stdint.h>
#include "meta.h"


namespace ObjectModel {

	//----------------------------------------------------------------------------------------------------
	class Root {
	protected:
		std::string name;
		int16_t     nameLength;
		//int8_t      wrapper;
		int32_t     size;
		int8_t      type;
	public:
		Root() :
			name("unknow"),
			wrapper(0),
			nameLength(0),
			size(sizeof(nameLength) + sizeof(wrapper) + sizeof(size)) { }; //protected 
		
		//virtual void pack() = 0; 
	public:
		inline int32_t  getSize() const { return this->size; };
		int8_t      wrapper;
		void  setName(std::string name) {
			this->name = name;
			nameLength = (int16_t)name.length();
			size += nameLength;
		}

		std::string  getName() const { return this->name; };

		virtual void pack(std::vector<int8_t>*, int16_t*) = 0;

	};
}      //namespace ObjectModel

