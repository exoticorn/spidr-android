#ifndef EXO_FILE_HPP
#define EXO_FILE_HPP

#include "exo/base/types.hpp"
#include <stdio.h>

namespace exo {
	class File {
	public:
		enum Access {
			Access_Read,
			Access_Write
		};

		File(const char* pFilename, Access access = Access_Read);
		~File();

		bool				isOpened() const { return m_pFile != nullptr; }
		uint				getSize() const { return m_size; }

		void				read(void* pDest, uint size);
		void				write(const void* pDest, uint size);

		uint8				readUint8();
		sint8				readSint8();
		uint16				readUint16();
		sint16				readSint16();
		uint32				readUint32();
		sint32				readSint32();
		float				readFloat();

		void				writeUint8(uint8 value);
		void				writeSint8(sint8 value) { writeUint8((uint8)value); }
		void				writeUint16(uint16 value);
		void				writeSint16(sint16 value) { writeUint16((uint16)value); }
		void				writeUint32(uint32 value);
		void				writeSint32(sint32 value) { writeUint32((uint32)value); }
		void				writeFloat(float value);

	private:
		FILE*				m_pFile;
		uint				m_size;
	};
}

#endif
