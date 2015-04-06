#ifndef __DATA_LUMP_H
#define __DATA_LUMP_H
#include "dataReader.h"
#include "dataWriter.h"
#include "fileLikeObject.h"


template<unsigned long size>
class DataLump: public FileLikeObject
                                   
{
public:
     DataLump(): w(rawData), 
                 r(rawData,size)
    {
        w.DataWriter::Fill(0,'\0',size);
    }
    virtual ~DataLump() = default;
    const unsigned char* RawData() const;
    unsigned char* RawData();

    // Writer
    inline virtual void Write(long offset, const void *src, long len) {
        w.DataWriter::Write(offset,src,len);
    }

    inline virtual void Put(long offset, unsigned char c) {
        w.DataWriter::Put(offset,c);
    }

    inline virtual void Fill(long offset, unsigned char c, long count) {
        w.DataWriter::Fill(offset,c,count);
    }

    virtual void Flush( ) {};

	unsigned char * RawPtr() { 
        return rawData;
    }
    
    // Reader
    inline virtual void Read(long offset, void *dest, long len) const {
        r.DataReader::Read(offset,dest,len);
    }

    inline virtual void ReadString(long offset, std::string& dest) const {
        r.DataReader::ReadString(offset,dest);
    }

    inline virtual unsigned char Get(long offset) const {
        return  rawData[offset];
    }

    inline virtual long Size() const {
        return size;
    } 

    virtual long Next( long offset, unsigned char c) const {
        return r.DataReader::Next(offset,c);
    }

    virtual long Last( long offset, unsigned char c) const {
        return r.DataReader::Next(offset,c);
    }

private:
     unsigned char rawData[size];
     DataWriter w;
     DataReader r;
};

class DataIO: public FileLikeObject
                                   
{
public:
     DataIO(unsigned char* rawData, long size)
         : w(rawData), r(rawData,size){}

    // Writer
    inline virtual void Write(long offset, const void *src, long len) {
        w.DataWriter::Write(offset,src,len);
    }

    inline virtual void Put(long offset, unsigned char c) {
        w.DataWriter::Put(offset,c);
    }

    inline virtual void Fill(long offset, unsigned char c, long count) {
        w.DataWriter::Fill(offset,c,count);
    }

    virtual void Flush( ) {};

	unsigned char * RawPtr() { 
        return w.DataWriter::RawPtr();
    }
    
    // Reader
    inline virtual void Read(long offset, void *dest, long len) const {
        r.DataReader::Read(offset,dest,len);
    }

    inline virtual void ReadString(long offset, std::string& dest) const {
        r.DataReader::ReadString(offset,dest);
    }

    inline virtual unsigned char Get(long offset) const {
        return  r.DataReader::Get(offset);
    }

    inline virtual long Size() const {
        return r.DataReader::Size();
    } 

    virtual long Next( long offset, unsigned char c) const {
        return r.DataReader::Next(offset,c);
    }

    virtual long Last( long offset, unsigned char c) const {
        return r.DataReader::Next(offset,c);
    }
private: 
     DataWriter w;
     DataReader r;
};
#endif
