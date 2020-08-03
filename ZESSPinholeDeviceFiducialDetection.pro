# -----------------------------------------------------------------------------
# ZESSPinholeDeviceFiducialDetection project profile
#
# \file
# \author  Alvarez
# \date    2020-07-23
# -----------------------------------------------------------------------------


TEMPLATE   = lib
TARGET     = ZESSPinholeDeviceFiducialDetection

DESTDIR    = $$(MLAB_CURRENT_PACKAGE_DIR)/lib
DLLDESTDIR = $$(MLAB_CURRENT_PACKAGE_DIR)/lib

# Set high warn level (warn 4 on MSVC)
WARN = HIGH

# Add used projects here (see included pri files below for available projects)
CONFIG += dll ML Base OpenCV

MLAB_PACKAGES += CARM_General \
                 MeVisLab_Standard

# make sure that this file is included after CONFIG and MLAB_PACKAGES
include ($(MLAB_MeVis_Foundation)/Configuration/IncludePackages.pri)

DEFINES += ZESSPINHOLEDEVICEFIDUCIALDETECTION_EXPORTS

# Enable ML deprecated API warnings. To completely disable the deprecated API, change WARN to DISABLE.
DEFINES += ML_WARN_DEPRECATED

HEADERS += \
    ZESSPinholeDeviceFiducialDetectionInit.h \
    ZESSPinholeDeviceFiducialDetectionSystem.h \
    mlPinholeDeviceFiducialDetection.h \

SOURCES += \
    ZESSPinholeDeviceFiducialDetectionInit.cpp \
    mlPinholeDeviceFiducialDetection.cpp \
	imageProcessing.cpp