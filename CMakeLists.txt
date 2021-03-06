project(QtFlow)
cmake_minimum_required(VERSION 3.2.0)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5Core)
set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "DEBUG")

option(BUILD_FULL_EDA "Build the full EDA" ON)
option(BUILD_STAND_ALONE_BINARIES "Build standalone binaries for testint EDA modules" ON)

include(GNUInstallDirs)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(KF5TextEditor REQUIRED)

set (CMAKE_CXX_STANDARD 11)
set(QT5_LIBRARIES
	Qt5::Widgets
	Qt5::Core
	Qt5::OpenGL
	Qt5::Gui
	KF5::TextEditor
)

if ("${LIB64}" STREQUAL "TRUE")
	set(LIBSUFFIX 64)
else()
	set(LIBSUFFIX "")
endif()

set(INSTALL_LIB_DIR lib${LIBSUFFIX} CACHE PATH "Installation directory for libraries")
mark_as_advanced(INSTALL_LIB_DIR)

if(BUILD_FULL_EDA)
	find_package(PythonLibs REQUIRED)
	#-----------------------------------------------------------------------------
	# Find Python
	find_package(PythonLibs REQUIRED)
	include_directories(${PYTHON_INCLUDE_DIRS})
	add_definitions(-DPYTHONQT_USE_RELEASE_PYTHON_FALLBACK)
	#-----------------------------------------------------------------------------
	# Library Name
	# To maintain ABI compatibility, the Qt major version as well as the Python major and minor version are needed
	string(SUBSTRING ${PYTHONLIBS_VERSION_STRING} 0 1 python_major)
	string(SUBSTRING ${PYTHONLIBS_VERSION_STRING} 2 1 python_minor)
	set(PythonQt Qt${QT_VERSION_MAJOR}Python${python_major}${python_minor} CACHE INTERNAL "")
	add_subdirectory(PythonQt)
endif(BUILD_FULL_EDA)

set(CMAKE_AUTOMOC ON)

if(BUILD_FULL_EDA)
	add_subdirectory(QtFlow)
endif(BUILD_FULL_EDA)

add_subdirectory(qeditor)
add_subdirectory(qlayout)
add_subdirectory(tech_reader)
add_subdirectory(def)
add_subdirectory(lef)
add_subdirectory(schematics)
add_subdirectory(schematics_reader)
add_subdirectory(qschematics)
add_subdirectory(magic)
add_subdirectory(magic3d)
add_subdirectory(contacts)
add_subdirectory(vcd)
add_subdirectory(vcd_reader)
#add_subdirectory(tech)
#add_subdirectory(blif)
add_subdirectory(cmap_reader)
add_subdirectory(gds2)
add_subdirectory(helpers)
add_subdirectory(symbol_reader)
add_subdirectory(symbols)
#add_subdirectory(coriolis)

install(DIRECTORY DESTINATION ${CMAKE_INSTALL_DATADIR}/qtflow)
install(FILES
	tech/ls1u.toml
	tech/ls1u.xml
	tech/mos.cmap
	tech/mos.dstyle
	tech/process.xml
	tech/scmos.par
	tech/scmos.tech
	DESTINATION
	${CMAKE_INSTALL_DATADIR}/qtflow/
)

install(DIRECTORY DESTINATION ${CMAKE_INSTALL_DATADIR}/qtflow/osu018)
install(FILES
	tech/osu018/SCN6M_SUBM.10.tech
	tech/osu018/osu018.magicrc
	tech/osu018/osu018.par
	tech/osu018/osu018.prm
	tech/osu018/osu018.sh
	tech/osu018/osu018_stdcells.gds
	tech/osu018/osu018_stdcells.lef
	tech/osu018/osu018_stdcells.lib
	tech/osu018/osu018_stdcells.slib
	tech/osu018/osu018_stdcells.sp
	tech/osu018/osu018_stdcells.v
	tech/osu018/osu035_pads.gds2
	DESTINATION
	${CMAKE_INSTALL_DATADIR}/qtflow/osu018
)

install(DIRECTORY DESTINATION ${CMAKE_INSTALL_DATADIR}/qtflow/osu035)
install(FILES
	tech/osu035/SCN4M_SUBM.20.tech
	tech/osu035/osu035.magicrc
	tech/osu035/osu035.par
	tech/osu035/osu035.prm
	tech/osu035/osu035.sh
	tech/osu035/osu035_pads.gds2
	tech/osu035/osu035_stdcells.gds2
	tech/osu035/osu035_stdcells.lef
	tech/osu035/osu035_stdcells.lib
	tech/osu035/osu035_stdcells.slib
	tech/osu035/osu035_stdcells.sp
	tech/osu035/osu035_stdcells.v
	DESTINATION
	${CMAKE_INSTALL_DATADIR}/qtflow/osu035
)

install(DIRECTORY DESTINATION ${CMAKE_INSTALL_DATADIR}/qtflow/osu050)
install(FILES
	tech/osu050/SCN3ME_SUBM.30.tech
	tech/osu050/osu050.magicrc
	tech/osu050/osu050.par
	tech/osu050/osu050.prm
	tech/osu050/osu050.sh
	tech/osu050/osu050_stdcells.lef
	tech/osu050/osu050_stdcells.sp
	tech/osu050/osu05_stdcells.lib
	tech/osu050/osu05_stdcells.v
	DESTINATION
	${CMAKE_INSTALL_DATADIR}/qtflow/osu050
)

install(DIRECTORY DESTINATION ${CMAKE_INSTALL_DATADIR}/qtflow/sxlib)
install(FILES
	tech/sxlib/sxlib.alliance.lib
	tech/sxlib/sxlib.lef
	tech/sxlib/sxlib.lib
	tech/sxlib/sxlib.slib
	DESTINATION
	${CMAKE_INSTALL_DATADIR}/qtflow/sxlib
)
