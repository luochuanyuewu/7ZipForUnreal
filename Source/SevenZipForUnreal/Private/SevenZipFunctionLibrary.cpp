// Fill out your copyright notice in the Description page of Project Settings.
#include "SevenZipFunctionLibrary.h"
#include "Runtime/Projects/Public/Interfaces/IPluginManager.h"
#include "bit7z.hpp"
#include "Paths.h"

using namespace bit7z;
using namespace BitFormat;
namespace {
	//Private static vars
	Bit7zLibrary* SZLib = nullptr;


	//Utility functions
	FString PluginRootFolder()
	{
		return IPluginManager::Get().FindPlugin("SevenZipForUnreal")->GetBaseDir();
	}

	FString ReversePathSlashes(FString forwardPath)
	{
		return forwardPath.Replace(TEXT("\\"), TEXT("/"));
	}

	bool IsValidDirectory(FString& directory, FString& fileName, const FString& path)
	{
		bool found = path.Split(TEXT("/"), &directory, &fileName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		//try a back split
		if (!found)
		{
			found = path.Split(TEXT("\\"), &directory, &fileName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		}

		//No valid directory found
		if (!found)
			return false;
		else
			return true;
	}

	FString DLLPath()
	{
		FString dllString = FString("7z.dll");
		return FPaths::ConvertRelativePathToFull(FPaths::Combine(*PluginRootFolder(), TEXT("ThirdParty/7zip/Dll"), *dllString));
	}

	using namespace std;

	std::wstring StringToWString(const std::string& str) 
	{
		int size_str = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
		wchar_t *wide = new wchar_t[size_str];
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wide, size_str);
		std::wstring return_wstring(wide);
		delete[] wide;
		wide = NULL;
		return return_wstring;
	}

	std::vector<uint8> TArrayToVector(const TArray<uint8>& InBuffer) 
	{
		std::vector<uint8> OutBuffer(InBuffer.Num());

		for (int i=0;i< InBuffer.Num();i++)
		{
			OutBuffer[i] = InBuffer[i];
		}

		return OutBuffer;
	}

	TArray<uint8> VectorToTArray(const vector<uint8>& InBuffer)
	{
		TArray<uint8> OutBuffer;
		for (uint8 e: InBuffer)
		{
			OutBuffer.Add(e);
		}

		return OutBuffer;
		
	}
}




USevenZipFunctionLibrary::USevenZipFunctionLibrary(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	UE_LOG(LogTemp, Log, TEXT("DLLPath is: %s"), *DLLPath());

	wstring wstr = StringToWString(TCHAR_TO_UTF8(*DLLPath()));
	SZLib = new Bit7zLibrary(wstr);
}

USevenZipFunctionLibrary::~USevenZipFunctionLibrary()
{
	SZLib->~Bit7zLibrary();
	SZLib = nullptr;
}



bool USevenZipFunctionLibrary::Extract(const FString& ArchivePath, const FString& DistPath)
{
	if (ArchivePath.IsEmpty())
	{
		return false;
	}

	FString t_dir;
	FString t_name;

	//Check directory validity
	if (!IsValidDirectory(t_dir, t_name, ArchivePath))
		return false;

	wstring t_archivePath = StringToWString(TCHAR_TO_UTF8(*ArchivePath));
	wstring t_distPath;

	if (DistPath.IsEmpty())
	{
		t_distPath = StringToWString(TCHAR_TO_UTF8(*t_dir));
	}
	else
	{
		t_distPath = StringToWString(TCHAR_TO_UTF8(*DistPath));
	}

	BitExtractor extractor(*SZLib, BitFormat::Zip);
	extractor.extract(t_archivePath,t_distPath);
	return true;
}

bool USevenZipFunctionLibrary::ExtractWithPwd(const FString& ArchivePath, const FString& DistPath, const FString& Password)
{
	if (ArchivePath.IsEmpty() || Password.IsEmpty())
	{
		return false;
	}
	
	FString t_dir;
	FString t_name;

	//Check directory validity
	if (!IsValidDirectory(t_dir, t_name, ArchivePath))
		return false;

	wstring t_archivePath = StringToWString(TCHAR_TO_UTF8(*ArchivePath));
	wstring t_distPath;
	wstring t_pwd = StringToWString(TCHAR_TO_UTF8(*Password));

	//detect dispath.
	if (DistPath.IsEmpty())
	{
		t_distPath = StringToWString(TCHAR_TO_UTF8(*t_dir));
	}
	else
	{
		t_distPath = StringToWString(TCHAR_TO_UTF8(*DistPath));
	}

	BitExtractor extractor(*SZLib, BitFormat::Zip);

	extractor.setPassword(t_pwd);

	extractor.extract(t_archivePath, t_distPath);

	return true;
}

bool USevenZipFunctionLibrary::ExtractAsBuffer(const FString& ArchivePath, TArray<uint8>& OutBuffer, int32 Index)
{
	if (ArchivePath.IsEmpty())
	{
		return false;
	}

	wstring t_archivePath = StringToWString(TCHAR_TO_UTF8(*ArchivePath));

	BitExtractor extractor(*SZLib, BitFormat::Zip);

	vector<byte_t> buffers;

	extractor.extract(t_archivePath, buffers, (uint32)Index);

	UE_LOG(LogTemp, Warning, TEXT("Vector Size = %d\n"), buffers.size());

	OutBuffer = VectorToTArray(buffers);

	UE_LOG(LogTemp, Warning, TEXT("Tarray Size = %d\n"), OutBuffer.Num());
	return true;
}

bool USevenZipFunctionLibrary::ExtractAsBufferWihtPwd(const FString& ArchivePath, TArray<uint8>& OutBuffer, const FString& Password,int32 Index /*= 0*/)
{
	if (ArchivePath.IsEmpty() || Password.IsEmpty())
	{
		return false;
	}

	wstring t_archivePath = StringToWString(TCHAR_TO_UTF8(*ArchivePath));
	wstring t_pwd = StringToWString(TCHAR_TO_UTF8(*Password));

	BitExtractor extractor(*SZLib, BitFormat::Zip);

	extractor.setPassword(t_pwd);

	vector<byte_t> buffers;

	extractor.extract(t_archivePath, buffers, (uint32)Index);

	UE_LOG(LogTemp, Warning, TEXT("Vector Size = %d\n"), buffers.size());

	OutBuffer = VectorToTArray(buffers);

	UE_LOG(LogTemp, Warning, TEXT("Tarray Size = %d\n"), OutBuffer.Num());
	return true;
}


