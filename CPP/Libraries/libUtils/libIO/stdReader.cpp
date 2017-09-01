#include "stdReader.h"

StdReader::StdReader(istream&f ): file(f) {
    file.seekg(0, file.end);
    length = file.tellg();
    file.seekg(file.end, file.beg);
    file.clear();
    pos = file.tellg();
}
void StdReader::Read(long offset, void *dest, long size) const {
    Seek(offset);
    file.read((char *)dest,size);
    UpdatePosition(size);
}
void StdReader::ReadString(long offset, std::string& dest) const {
    Seek(offset);

    getline(file,dest,'\0');

    // Don't know where position is, we'll have to manually look it up next time...
    pos = -1;
}
unsigned char StdReader::Get(long offset) const {
    Seek(offset);

    unsigned char c = static_cast<unsigned char>(file.get());
    UpdatePosition(1);

    return c;
}

long StdReader::Size() const {
    return length;
}

long StdReader::Next( long offset, unsigned char c) const {
    Seek(offset); 

    char buf[1024];
    unsigned char current = file.peek();

    while ( current != c && file.good() ) {
        file.get(buf,1024,c);
        UpdatePosition(file.gcount());
        current = file.peek();
    }

    file.clear();
    
    if ( current == c ) {
        return pos;
    } else {
        return Size();
    }
}
long StdReader::Last( long offset, unsigned char c) const {
    for (long i=offset; i>=0; i--) {
		// TODO: do some kind of buffering here?
		this->file.seekg(i, ios_base::beg);
		unsigned char got = static_cast<unsigned char>(file.get());
		if ( got == c ) {
			pos = offset - i;
			return i;
		}
    }

    pos = -1;
    return -1;
}

void StdReader::UpdatePosition(int add) const {
    if ( pos != -1 && file.good() ) {
        pos+=add;
    } else {
        pos = -1; // Something's gone wrong - we need to manually seek next time
    }
}

void StdReader::Seek(int offset) const {
    if ( pos != offset ) {
        this->file.seekg(offset, ios_base::beg);
        pos = offset;
    }
}

IFStreamReader::IFStreamReader(const char *fname): 
                ifstream(fname, ios_base::binary | ios_base::in),
                StdReader((ifstream& )*this),
				fileName(fname)
{
}

/*
 * TODO: This is working around a gcc bug - should improve when move
 *       constructors are implemented
 */
IFStreamReader::IFStreamReader(IFStreamReader&& from )
    : ifstream(from.fileName), 
      StdReader( (ifstream& ) *this)
{
    std::swap(fileName,from.fileName);
}

