#define EXPORT __declspec(dllexport)

EXPORT bool writeData(wchar_t* region, wchar_t* bucketName, wchar_t* directory, wchar_t* filename, wchar_t* data);