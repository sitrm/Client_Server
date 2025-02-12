#pragma once

#include <stdint.h>
#include <memory>
#include "core.h"
#include "root.h"

namespace ObjectModel {

	class Primitive : public Root {
	private:
		int8_t               type = 0;
		std::vector<int8_t>* data = nullptr;
	private:
		Primitive() {         
			size += sizeof(type);
		}
	public:
		std::vector<int8_t>* getPtrData() const { return data; }
		std::vector<int8_t>  getData()    const { return *data;}

		template<typename T>
		static Primitive* create(std::string name, Type type, T value) {
			
			Primitive* p = new Primitive();
			p->setName(name);
			p->wrapper = static_cast<int8_t>(Wrapper::PRIMITIVE);     
			p->type = static_cast<int8_t>(type);
			p->data = new std::vector<int8_t>(sizeof(value));   
			p->size += (int32_t)p->data->size();                
			
			
			int16_t iterator = 0;
			Core::template encode(p->data, &iterator, value); 

			return p;
		}

		void pack(std::vector<int8_t>* buffer, int16_t* iterator);

		static Primitive unpack(const std::vector<int8_t>* buffer, int16_t* it);

		static Primitive unpack(const std::vector<int8_t>* buffer);

	};

}
