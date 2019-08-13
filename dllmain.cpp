#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <fstream>

#include "dllmain.h"

using AwsStringUtils = Aws::Utils::StringUtils;

bool putS3Object(const Aws::String& s3Region, const Aws::String& s3BucketName, const Aws::String& s3ObjectName, const Aws::String& data)
{
	Aws::Client::ClientConfiguration clientConfiguration;
	clientConfiguration.region = s3Region;

	Aws::S3::S3Client s3Client(clientConfiguration);
	Aws::S3::Model::PutObjectRequest objectRequest;

	objectRequest.SetBucket(s3BucketName);
	objectRequest.SetKey(s3ObjectName);

	auto inputData = Aws::MakeShared<Aws::StringStream>("PutObjectInputStream");
	
	inputData->write(data.c_str(), data.size());

	objectRequest.SetBody(inputData);
	
	auto putObjectOutcome = s3Client.PutObject(objectRequest);

	if (!putObjectOutcome.IsSuccess()) {
		auto error = putObjectOutcome.GetError();
		std::cout << "ERROR: " << error.GetExceptionName() << ": "
			<< error.GetMessage() << ": "
			<< std::endl;
	}

	return putObjectOutcome.IsSuccess();
}

EXPORT bool writeData(wchar_t *region, wchar_t *bucketName, wchar_t *directory, wchar_t* filename, wchar_t *data)
{
	Aws::SDKOptions options;
	options.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Trace;
	bool success = false;

	Aws::InitAPI(options);

	{		
		const Aws::String s3BucketName(AwsStringUtils::FromWString(bucketName));
		const Aws::String s3Region(AwsStringUtils::FromWString(region));
		const Aws::String s3Data(AwsStringUtils::FromWString(data));

		std::wstring filePath = directory;
		filePath.append(L"/");
		filePath.append(filename);

		const Aws::String s3ObjectName = AwsStringUtils::FromWString(filePath.c_str());

		success = putS3Object(s3Region, s3BucketName, s3ObjectName, s3Data);
	}

	Aws::ShutdownAPI(options);

	return success;
}