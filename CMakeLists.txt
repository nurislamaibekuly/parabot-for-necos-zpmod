cmake_minimum_required(VERSION 3.9)
project(Parabot)

# Parabot is a bot mod for Half-Life and friends, vendored here with a small
# patch that teaches it about necos-zpmod (see dlls/h_export.cpp and
# bot/bot.h). Built as a drop-in replacement for the game dll: it loads
# zpserver itself, or serves as a Metamod plugin.

set(PARABOT_SOURCES
	bot/bot.cpp
	bot/bot_client.cpp
	bot/marker.cpp
	bot/parabot.cpp
	bot/pb_action.cpp
	bot/pb_cell.cpp
	bot/pb_chat.cpp
	bot/pb_combat.cpp
	bot/pb_combatgoals.cpp
	bot/pb_configuration.cpp
	bot/pb_focus.cpp
	bot/pb_global.cpp
	bot/pb_goalfinder.cpp
	bot/pb_goals.cpp
	bot/pb_journey.cpp
	bot/pb_kills.cpp
	bot/pb_mapcells.cpp
	bot/pb_mapgraph.cpp
	bot/pb_mapimport.cpp
	bot/pb_navpoint.cpp
	bot/pb_needs.cpp
	bot/pb_observer.cpp
	bot/pb_path.cpp
	bot/pb_perception.cpp
	bot/pb_roaming.cpp
	bot/pb_sectors.cpp
	bot/pb_vistable.cpp
	bot/pb_weapon.cpp
	bot/pb_weaponhandling.cpp
	bot/pbt_dynarray.cpp
	bot/pbt_priorityqueue.cpp
	bot/sounds.cpp
	bot/utilityfuncs.cpp
	bot/zz_trace.cpp
	dlls/commands.cpp
	dlls/dll.cpp
	dlls/engine.cpp
	dlls/h_export.cpp
	dlls/linkfunc.cpp
	dlls/startframe.cpp
	dlls/util.cpp
	metamod/meta_api.cpp
)

if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE)
else()
	add_compile_options(-fno-rtti)
	# upstream is C++98-era code relying on GCC's permissive aggregate
	# initialization (e.g. double constants in bool members)
	add_compile_options(-Wno-c++11-narrowing)
endif()

add_library(parabot SHARED ${PARABOT_SOURCES})

target_include_directories(parabot PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/bot"
	"${CMAKE_CURRENT_SOURCE_DIR}/common"
	"${CMAKE_CURRENT_SOURCE_DIR}/dlls"
	"${CMAKE_CURRENT_SOURCE_DIR}/engine"
	"${CMAKE_CURRENT_SOURCE_DIR}/metamod"
	"${CMAKE_CURRENT_SOURCE_DIR}/pm_shared"
	"${CMAKE_CURRENT_SOURCE_DIR}/utils"
)

if(WIN32)
	# stable export ordinals; the def also gives MinGW the undecorated
	# GiveFnptrsToDll name that --add-stdcall-alias would provide
	target_sources(parabot PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Parabot.def")
elseif(UNIX AND NOT APPLE)
	target_link_libraries(parabot PRIVATE dl)
endif()

include(LibraryNaming)
set_target_postfix_with_name(parabot "parabot")

install(TARGETS parabot
	RUNTIME DESTINATION "${GAMEDIR}/${SERVER_INSTALL_DIR}/"
	LIBRARY DESTINATION "${GAMEDIR}/${SERVER_INSTALL_DIR}/"
	PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
		GROUP_READ GROUP_EXECUTE
		WORLD_READ WORLD_EXECUTE)

# language files and the readme; bot configs are created on first run
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/addons/parabot"
	DESTINATION "${GAMEDIR}/addons")
