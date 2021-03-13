#pragma once
#include <memory>

//Base Document class as abstract class with virtual Destructor(reguired if instantiated as unique_ptr, in case of shared_ptr it is not required)

class Document
{
public:
	virtual void Read() = 0;
	virtual void Write() = 0;
	virtual ~Document() = default;
};

using DocumentPtr = std::unique_ptr<Document>; //alias for Document pointer

