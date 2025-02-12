#pragma once
#include <iostream>
#include <stdint.h>
#include "primitive.h"
#include "array.h"
#include "core.h"
#include "root.h"

namespace ObjectModel {

	class Object : public Root {
	private:


	public:
		//refactor // std::vector<Root*> entities;     
		int16_t primitiveCount = 0, arrayCount = 0, stringCount = 0, objectCount = 0;
		std::vector<Primitive> primitives;
		std::vector<Array> arrays;
		std::vector<Array> strings;
		std::vector<Object> objects;


		Object(std::string name = "default") {
			setName(name);
			wrapper = static_cast<int8_t>(Wrapper::OBJECT);
			size += (sizeof(int16_t)) * 4;
		}

		inline int16_t getPrimitiveCount() const { return this->primitiveCount; }
		inline int16_t getArrayCount()     const { return this->arrayCount; }
		inline int16_t getStringCount()    const { return this->stringCount; }
		inline int16_t getObjectCount()    const { return this->objectCount; }
		std::string    getName()           const { return this->name; }


		void addEntity(Root* r);

		Root* findByName(std::string name);

		void pack(std::vector<int8_t>* buffer, int16_t* it);

		//deserialization-------------------------------------------------

		static Object unpack(std::vector<int8_t>* buffer, int16_t* it);

	};

}