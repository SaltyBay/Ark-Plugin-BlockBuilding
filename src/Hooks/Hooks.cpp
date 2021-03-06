/*****************************************************************************************************************
* Copyright (C) 2019 by Matthias Birnthaler                                                                      *
*                                                                                                                *
* This file is part of the TribeSlotCooldown Plugin for Ark Server API                                           *
*                                                                                                                *
*    This program is free software : you can redistribute it and/or modify                                       *
*    it under the terms of the GNU General Public License as published by                                        *
*    the Free Software Foundation, either version 3 of the License, or                                           *
*    (at your option) any later version.                                                                         *
*                                                                                                                *
*    This program is distributed in the hope that it will be useful,                                             *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of                                              *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the                                                 *
*    GNU General Public License for more details.                                                                *
*                                                                                                                *
*    You should have received a copy of the GNU General Public License                                           *
*    along with this program.If not, see <https://www.gnu.org/licenses/>.                                        *
*                                                                                                                *
*****************************************************************************************************************/

/**
* \file Hooks.cpp
* \author Matthias Birnthaler
* \date 15 May 2019
* \brief File containing the implementation for all needed Hooks
*
*/


/* ================================================[includes]================================================ */
#include "Hooks.h"


/* ===================================== [prototype of local functions] ======================================= */

static int  Hook_APrimalStructure_IsAllowedToBuild(APrimalStructure* _this, APlayerController* PC, FVector AtLocation, FRotator AtRotation, FPlacementData* OutPlacementData,
	bool bDontAdjustForMaxRange, FRotator PlayerViewRotation, bool bFinalPlacement);

static FString GetBlueprint(UObjectBase* object);

static FString GetBlueprint(UObjectBase* object)
{
	if (object != nullptr && object->ClassField() != nullptr)
	{
		FString path_name;
		object->ClassField()->GetDefaultObject(true)->GetFullName(&path_name, nullptr);

		if (int find_index = 0; path_name.FindChar(' ', find_index))
		{
			path_name = "Blueprint'" + path_name.Mid(find_index + 1,
				path_name.Len() - (find_index + (path_name.EndsWith(
					"_C", ESearchCase::
					CaseSensitive)
					? 3
					: 1))) + "'";
			return path_name.Replace(L"Default__", L"", ESearchCase::CaseSensitive);
		}
	}

	return FString("");
}


/**
* \brief Hook_AShooterGameMode_RemovePlayerFromTribe
*
* This function hooks the RemovePlayerFromTribe Implementation. If the removed player is no server admin a Player slot from
* the tribe is set on cooldown 
*
* \param[in] _this AShooterGameMode, variable is not used 
* \param[in] TribeID the id of the tribe where a player gets removed
* \param[in] PlayerDataID the id of the removed player 
* \param[in] bDontUpdatePlayerState, variable is not used 
* \return void
*/
static int  Hook_APrimalStructure_IsAllowedToBuild(APrimalStructure* _this, APlayerController* PC, FVector AtLocation, FRotator AtRotation, FPlacementData* OutPlacementData, 
	bool bDontAdjustForMaxRange, FRotator PlayerViewRotation, bool bFinalPlacement)
{
	if ((nullptr != _this) && (nullptr != PC))
	{
		const FString path_name = GetBlueprint(_this);

		if ((path_name == FString("Blueprint'/Game/Mods/StructuresPlusMod/Structures/FenceSupports/Stone/BP_FenceSupport_Stone.BP_FenceSupport_Stone'")) || 
			(path_name == FString("Blueprint'/Game/Mods/StructuresPlusMod/Structures/FenceSupports/Wood/BP_FenceSupport_Wood.BP_FenceSupport_Wood'")) ||
			(path_name == FString("Blueprint'/Game/Mods/StructuresPlusMod/Structures/FenceSupports/Adobe/BP_FenceSupport_Adobe.BP_FenceSupport_Adobe'")) ||
			(path_name == FString("Blueprint'/Game/Mods/StructuresPlusMod/Structures/FenceSupports/Tek/BP_FenceSupport_Tek.BP_FenceSupport_Tek'")) ||
			(path_name == FString("Blueprint'/Game/Mods/StructuresPlusMod/Structures/FenceSupports/Metal/BP_FenceSupport_Metal.BP_FenceSupport_Metal'")) )
		{
			ArkApi::GetApiUtils().SendNotification(static_cast<AShooterPlayerController*>(PC),
				FLinearColor(1, 0, 0),
				1.4f, 30, nullptr, "It is not allowed to use this structure !!!");

			return 0;
		}

		if (path_name == FString("Blueprint'/Game/Mods/StructuresPlusMod/Structures/Foundations/Square/Metal/BP_Foundation_Metal.BP_Foundation_Metal'"))
		{
			ArkApi::GetApiUtils().SendNotification(static_cast<AShooterPlayerController*>(PC),
				FLinearColor(1, 0, 0),
				1.4f, 30, nullptr, "S+ foundations are not allowed for spam!!! Use for base building only!!!");
		}
	
	
	}

	return APrimalStructure_IsAllowedToBuild_original(_this, PC, AtLocation, AtRotation, OutPlacementData, bDontAdjustForMaxRange, PlayerViewRotation, bFinalPlacement);
}



/* ===================================== [definition of global functions] ===================================== */

/**
* \brief Initialisation of needed Hooks
*
* This function initialise all needed Hooks
*
* \return void
*/
void InitHooks(void)
{
	ArkApi::GetHooks().SetHook("APrimalStructure.IsAllowedToBuild", 
		&Hook_APrimalStructure_IsAllowedToBuild, 
		&APrimalStructure_IsAllowedToBuild_original);

}


/**
* \brief Cancellation of needed Hooks
*
* This function removes all needed Hooks.
*
* \return void
*/
void RemoveHooks(void)
{
	ArkApi::GetHooks().DisableHook("APrimalStructure.IsAllowedToBuild", 
		&Hook_APrimalStructure_IsAllowedToBuild);

}

/* =================================================[end of file]================================================= */
