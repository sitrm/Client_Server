#pragma once
#include "root.h"
#include "meta.h"
#include "core.h"
#include <memory>
#include <stdint.h>

namespace ObjectModel {

	class Array : public Root {
	private:
		int8_t type = 0;
		int32_t count = 0;   
		std::vector<int8_t>* data = nullptr;
	private:
		Array() {
			size += sizeof(type) + sizeof(count);
		}
	public:

		template<typename T>
		static Array* createArray(std::string name, Type type, std::vector<T> value) {
			Array* arr = new Array();
			arr->setName(name);
			arr->wrapper = static_cast<int8_t>(Wrapper::ARRAY);
			arr->type = static_cast<int8_t>(type);
			arr->count = (int32_t)value.size();
			arr->data = new std::vector<int8_t>(arr->count * sizeof(T));
			arr->size += (int32_t)arr->count * sizeof(T);


			int16_t iterator = 0;
			Core::template encode<T>(arr->data, &iterator, value);

			return arr;
		}

		template<typename T>
		static Array* createString(std::string name, Type type, T value) {
			Array* str = new Array();
			str->setName(name);
			str->wrapper = static_cast<int8_t>(Wrapper::STRING);
			str->type = static_cast<int8_t>(type);
			str->count = (int32_t)value.size();
			str->data = new std::vector<int8_t>(value.size());
			str->size += (int32_t)value.size();


			int16_t iterator = 0;
			Core::template encode<T>(str->data, &iterator, value);

			return str;
		}

		void pack(std::vector<int8_t>* buffer, int16_t* iterator);

		static Array unpack(std::vector<int8_t>* buffer, int16_t* it);

		static Array unpackS(std::vector<int8_t>* buffer, int16_t* it);
	}; // class Array

} // namespace ObjectMOdel