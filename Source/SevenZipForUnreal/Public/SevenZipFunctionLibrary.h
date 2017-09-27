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
		static void Test();

	UFUNCTION(BlueprintCallable, Category = SevenZip)
		static bool Unzip(const FString& ArchivePath);

	UFUNCTION(BlueprintCallable, Category = SevenZip)
		static bool UnzipWithPwd(const FString& ArchivePath, const FString& Password);

	UFUNCTION(BlueprintCallable, Category = SevenZip)
		static bool UnzipTo(const FString& ArchivePath,const FString& DistPath);

	UFUNCTION(BlueprintCallable, Category = SevenZip)
		static bool UnzipToWithPwd(const FString& ArchivePath, const FString& DistPath ,const FString& Password);
};
