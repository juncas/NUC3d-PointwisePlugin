########################################################################
# Pointwise - Proprietary software product of Pointwise, Inc.
#             Copyright (c) 1995-2014 Pointwise, Inc.
#             All rights reserved.
#
# modulelocal.mk for src\plugins\NUC3d plugin
########################################################################

#-----------------------------------------------------------------------
# OPTIONAL, locally defined plugin make targets. If a plugin developer wants
# to extend a plugin's make scheme, they should create a modulelocal.mk file
# in the plugin's base folder. To provide for future SDK upgrades, the standard
# module.mk file should NOT be directly edited.
#
# Uncomment, copy and/or edit the sections below as needed.
#
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
# Adds plugin specific source files to the build.
# These files will be compiled and then linked to the plugin.
# The file paths are relative to the project path macro NUC3d_LOC.
# For example,
#    myFile.cxx is located in $(NUC3d_LOC)/myFile.cxx
#    sub/myOtherFile.cxx is located in $(NUC3d_LOC)/sub/myOtherFile.cxx
#
#NUC3d_CXXFILES_PRIVATE := \
#    yourfile1.cxx \
#    yourfile2.cxx \
#	$(NULL)

#-----------------------------------------------------------------------
# Adds plugin specific include flags to the build.
#
#NUC3d_INCL_PRIVATE := \
#	-I./path/to/a/private/include/folder \
#	-I./path/to/another/private/include/folder \
#	$(NULL)

#-----------------------------------------------------------------------
# Adds plugin specific compile flags to the build.
#
#NUC3d_CXXFLAGS_PRIVATE := \
#	$(NULL)


#-----------------------------------------------------------------------
# Adds plugin specific -lfile and -Llibpath to build.
# These flags will be added to the link.
#
#NUC3d_LIBS_PRIVATE := \
#	-L./path/to/a/private/lib/folder \
#	-lprivate \
#	-L./path/to/another/private/lib/folder \
#	-lprivate2 \
#	$(NULL)

#-----------------------------------------------------------------------
# Adds plugin specific link flags to the build.
#
#NUC3d_LDFLAGS_PRIVATE := \
#	$(NULL)

#-----------------------------------------------------------------------
# Add any locally defined targets that do NOT produce an output object.
# This would include targets used for cleaning, printing, etc. These targets
# will be automatically added the .PHONY target.
#
#NUC3d_MAINT_TARGETS_PRIVATE = \
#	$(NULL)

#-----------------------------------------------------------------------
# Sample macro. Prefix with CAE name to prevent conflicts.
#
#NUC3d_DUMMY = \
#	DUMMY1 \
#	DUMMY2 \
#	$(NULL)

#-----------------------------------------------------------------------
# Sample target. Prefix with CAE name to prevent conflicts.
#
#NUC3d_sample: NUC3d_clean NUC3d
#	@echo "done building NUC3d"
