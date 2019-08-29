#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <fstream>

#include "dllmain.h"

using AwsStringUtils = Aws::Utils::StringUtils;

Aws::SDKOptions options;

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

	return putObjectOutcome.IsSuccess();
}

EXPORT void initializeAwsApi()
{
	Aws::InitAPI(options);
}

EXPORT void shutdownAwsApi()
{
	Aws::ShutdownAPI(options);
}

EXPORT bool writeData(wchar_t *region, wchar_t *bucketName, wchar_t *directory, wchar_t* filename, wchar_t *data)
{	
	bool success = false;

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

	return success;
}