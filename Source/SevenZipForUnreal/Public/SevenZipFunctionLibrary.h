// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SevenZipFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SEVENZIPFORUNREAL_API USevenZipFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
	
	
public:
	~USevenZipFunctionLibrary();
	UFUNCTION(BlueprintCallable, Category = SevenZip)
		static bool Extract(const FString& ArchivePath,const FString& DistPath);

	UFUNCTION(BlueprintCallable, Category = SevenZip)
		static bool ExtractWithPwd(const FString& ArchivePath, const FString& DistPath ,const FString& Password);
	
	UFUNCTION(BlueprintCallable, Category = SevenZip)
		static bool ExtractAsBuffer(const FString& ArchivePath, TArray<uint8>& OutBuffer, int32 Index=0);

	UFUNCTION(BlueprintCallable, Category = SevenZip)
		static bool ExtractAsBufferWihtPwd(const FString& ArchivePath, TArray<uint8>& OutBuffer, const FString& Password, int32 Index = 0);
};
