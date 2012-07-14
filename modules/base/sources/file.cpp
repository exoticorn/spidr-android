#include "exo/base/file.hpp"
#include <stdlib.h>

namespace exo {
	File::File(const char* pFilename, Access access) {
		m_size = 0;
		m_pFile = fopen(pFilename, access == Access_Read ? "rb" : "wb");
		if(m_pFile != nullptr) {
			fseek(m_pFile, 0, SEEK_END);
			m_size = ftell(m_pFile);
			fseek(m_pFile, 0, SEEK_SET);
		}
	}

	File::~File() {
		if(m_pFile != nullptr) {
			fclose(m_pFile);
		}
	}

	void File::read(void* pDest, uint size) {
		while(size > 0) {
			int readBytes = fread(pDest, 1, size, m_pFile);
			if(readBytes == 0) {
				printf("Failed to read any bytes\n");
				exit(1);
			}
			size -= readBytes;
		}
	}

	void File::write(const void* pDest, uint size) {
		while(size > 0) {
			int writtenBytes = fwrite(pDest, 1, size, m_pFile);
			if(writtenBytes == 0) {
				printf("Failed to write any bytes\n");
				exit(1);
			}
			size -= writtenBytes;
		}
	}

	uint8 File::readUint8() {
		uint8 v;
		read(&v, sizeof(v));
		return v;
	}

	sint8 File::readSint8() {
		sint8 v;
		read(&v, sizeof(v));
		return v;
	}

	uint16 File::readUint16() {
		uint16 v;
		read(&v, sizeof(v));
		return v;
	}

	sint16 File::readSint16() {
		sint16 v;
		read(&v, sizeof(v));
		return v;
	}

	uint32 File::readUint32() {
		uint32 v;
		read(&v, sizeof(v));
		return v;
	}

	sint32 File::readSint32() {
		sint32 v;
		read(&v, sizeof(v));
		return v;
	}

	float File::readFloat() {
		float v;
		read(&v, sizeof(v));
		return v;
	}

	void File::writeUint8(uint8 v) {
		write(&v, sizeof(v));
	}

	void File::writeUint16(uint16 v) {
		write(&v, sizeof(v));
	}

	void File::writeUint32(uint32 v) {
		write(&v, sizeof(v));
	}
}
