#
# OMNeT++/OMNEST Makefile for libapplication
#
# This file was generated with the command:
#  opp_makemake -f --make-so -o application -O out -d msg -I/home/mani/Desktop/eigen -I../../veins/src/modules/messages -I../../veins/src/modules/obstacle -I../../veins/src/modules -I../../veins/src/base/modules -Imsg -I../../veins/src/modules/utility -I../../veins/src/modules/mobility -I../../veins/src/base/utils -I../../veins/src/base/messages -I../../veins/src/base/phyLayer -I../../veins/src/modules/mac -I../../veins/src/modules/phy -I../../veins/src/modules/mac/ieee80211p -I../../veins/src/base/connectionManager -I../../veins/src/modules/analogueModel -L../../veins/out/$(CONFIGNAME)/src/modules -L../../veins/out/$(CONFIGNAME)/tests/testUtils -L../../veins/out/$(CONFIGNAME)/src/base -L../out/$(CONFIGNAME)/application/msg -lmiximbase -lmiximmodules -lmsg -lcrypto -lssl -lmiximmodules -lmiximtestUtils -lmiximbase -KVEINS_PROJ=../../veins -- /usr/lib/libboost_serialization-mt.a
#

# Name of target to be created (-o option)
TARGET = libapplication$(SHARED_LIB_SUFFIX)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I/home/mani/Desktop/eigen \
    -I../../veins/src/modules/messages \
    -I../../veins/src/modules/obstacle \
    -I../../veins/src/modules \
    -I../../veins/src/base/modules \
    -Imsg \
    -I../../veins/src/modules/utility \
    -I../../veins/src/modules/mobility \
    -I../../veins/src/base/utils \
    -I../../veins/src/base/messages \
    -I../../veins/src/base/phyLayer \
    -I../../veins/src/modules/mac \
    -I../../veins/src/modules/phy \
    -I../../veins/src/modules/mac/ieee80211p \
    -I../../veins/src/base/connectionManager \
    -I../../veins/src/modules/analogueModel \
    -I.

# Additional object and library files to link with
EXTRA_OBJS = /usr/lib/libboost_serialization-mt.a

# Additional libraries (-L, -l options)
LIBS = -L../../veins/out/$(CONFIGNAME)/src/modules -L../../veins/out/$(CONFIGNAME)/tests/testUtils -L../../veins/out/$(CONFIGNAME)/src/base -L../out/$(CONFIGNAME)/application/msg  -lmiximbase -lmiximmodules -lmsg -lcrypto -lssl -lmiximmodules -lmiximtestUtils -lmiximbase
LIBS += -Wl,-rpath,`abspath ../../veins/out/$(CONFIGNAME)/src/modules` -Wl,-rpath,`abspath ../../veins/out/$(CONFIGNAME)/tests/testUtils` -Wl,-rpath,`abspath ../../veins/out/$(CONFIGNAME)/src/base` -Wl,-rpath,`abspath ../out/$(CONFIGNAME)/application/msg`

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = application
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/ApplV_C2C_Epidemic_Ideal.o \
    $O/Appl.o \
    $O/TraCI.o \
    $O/ApplV.o \
    $O/ApplV_C2C_Epidemic.o \
    $O/ApplV_ICE.o \
    $O/ApplV_MPB.o \
    $O/Statistics.o \
    $O/ApplV_Broadcast.o \
    $O/ApplV_NewMPB.o \
    $O/ApplV_ICEv2.o \
    $O/ApplCA.o \
    $O/ApplV_BBroadcast.o \
    $O/ApplRSU.o \
    $O/RSUmobility.o

# Message files
MSGFILES =

# Other makefile variables (-K)
VEINS_PROJ=../../veins

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppenvir$D $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $O/$(TARGET)
	$(Q)$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS) submakedirs $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	@echo Creating shared library: $@
	$(Q)$(SHLIB_LD) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS) $(LDFLAGS)
	$(Q)$(SHLIB_POSTPROCESS) $O/$(TARGET)

submakedirs:  msg_dir

.PHONY: all clean cleanall depend msgheaders  msg
msg: msg_dir

msg_dir:
	cd msg && $(MAKE) all

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)
	$(Q)cd msg && $(MAKE) msgheaders

clean:
	$(qecho) Cleaning...
	$(Q)-rm -rf $O
	$(Q)-rm -f application application.exe libapplication.so libapplication.a libapplication.dll libapplication.dylib
	$(Q)-rm -f ./*_m.cc ./*_m.h

	-$(Q)cd msg && $(MAKE) clean

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...
	$(Q)$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc
	$(Q)-cd msg && if [ -f Makefile ]; then $(MAKE) depend; fi

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/Appl.o: Appl.cc \
	Appl.h \
	ExtraClasses.h \
	msg/Beacon_m.h \
	msg/CRL_Piece_m.h \
	msg/SimpleMsg_m.h \
	$(VEINS_PROJ)/src/base/messages/MacPkt_m.h \
	$(VEINS_PROJ)/src/base/modules/BaseBattery.h \
	$(VEINS_PROJ)/src/base/modules/BaseLayer.h \
	$(VEINS_PROJ)/src/base/modules/BaseMacLayer.h \
	$(VEINS_PROJ)/src/base/modules/BaseModule.h \
	$(VEINS_PROJ)/src/base/modules/BatteryAccess.h \
	$(VEINS_PROJ)/src/base/phyLayer/Interpolation.h \
	$(VEINS_PROJ)/src/base/phyLayer/MappingBase.h \
	$(VEINS_PROJ)/src/base/utils/FindModule.h \
	$(VEINS_PROJ)/src/base/utils/HostState.h \
	$(VEINS_PROJ)/src/base/utils/MiXiMDefs.h \
	$(VEINS_PROJ)/src/base/utils/NetwControlInfo.h \
	$(VEINS_PROJ)/src/base/utils/PassedMessage.h \
	$(VEINS_PROJ)/src/base/utils/SimpleAddress.h \
	$(VEINS_PROJ)/src/base/utils/miximkerneldefs.h \
	$(VEINS_PROJ)/src/modules/mac/Mac80211.h \
	$(VEINS_PROJ)/src/modules/mac/Mac80211MultiChannel.h \
	$(VEINS_PROJ)/src/modules/messages/Mac80211Pkt_m.h \
	$(VEINS_PROJ)/src/modules/utility/Consts80211.h
$O/ApplCA.o: ApplCA.cc \
	Appl.h \
	ApplCA.h \
	ExtraClasses.h \
	msg/Beacon_m.h \
	msg/CRL_Piece_m.h \
	msg/SimpleMsg_m.h \
	$(VEINS_PROJ)/src/base/messages/MacPkt_m.h \
	$(VEINS_PROJ)/src/base/modules/BaseBattery.h \
	$(VEINS_PROJ)/src/base/modules/BaseLayer.h \
	$(VEINS_PROJ)/src/base/modules/BaseMacLayer.h \
	$(VEINS_PROJ)/src/base/modules/BaseModule.h \
	$(VEINS_PROJ)/src/base/modules/BatteryAccess.h \
	$(VEINS_PROJ)/src/base/phyLayer/Interpolation.h \
	$(VEINS_PROJ)/src/base/phyLayer/MappingBase.h \
	$(VEINS_PROJ)/src/base/utils/FindModule.h \
	$(VEINS_PROJ)/src/base/utils/HostState.h \
	$(VEINS_PROJ)/src/base/utils/MiXiMDefs.h \
	$(VEINS_PROJ)/src/base/utils/NetwControlInfo.h \
	$(VEINS_PROJ)/src/base/utils/PassedMessage.h \
	$(VEINS_PROJ)/src/base/utils/SimpleAddress.h \
	$(VEINS_PROJ)/src/base/utils/miximkerneldefs.h \
	$(VEINS_PROJ)/src/modules/mac/Mac80211.h \
	$(VEINS_PROJ)/src/modules/mac/Mac80211MultiChannel.h \
	$(VEINS_PROJ)/src/modules/messages/Mac80211Pkt_m.h \
	$(VEINS_PROJ)/src/modules/utility/Consts80211.h
$O/ApplRSU.o: ApplRSU.cc \
	Appl.h \
	ApplRSU.h \
	ExtraClasses.h \
	msg/Beacon_m.h \
	msg/CRL_Piece_m.h \
	msg/SimpleMsg_m.h \
	$(VEINS_PROJ)/src/base/messages/MacPkt_m.h \
	$(VEINS_PROJ)/src/base/modules/BaseBattery.h \
	$(VEINS_PROJ)/src/base/modules/BaseLayer.h \
	$(VEINS_PROJ)/src/base/modules/BaseMacLayer.h \
	$(VEINS_PROJ)/src/base/modules/BaseModule.h \
	$(VEINS_PROJ)/src/base/modules/BatteryAccess.h \
	$(VEINS_PROJ)/src/base/phyLayer/Interpolation.h \
	$(VEINS_PROJ)/src/base/phyLayer/MappingBase.h \
	$(VEINS_PROJ)/src/base/utils/FindModule.h \
	$(VEINS_PROJ)/src/base/utils/HostState.h \
	$(VEINS_PROJ)/src/base/utils/MiXiMDefs.h \
	$(VEINS_PROJ)/src/base/utils/NetwControlInfo.h \
	$(VEINS_PROJ)/src/base/utils/PassedMessage.h \
	$(VEINS_PROJ)/src/base/utils/SimpleAddress.h \
	$(VEINS_PROJ)/src/base/utils/miximkerneldefs.h \
	$(VEINS_PROJ)/src/modules/mac/Mac80211.h \
	$(VEINS_PROJ)/src/modules/mac/Mac80211MultiChannel.h \
	$(VEINS_PROJ)/src/modules/messages/Mac80211Pkt_m.h \
	$(VEINS_PROJ)/src/modules/utility/Consts80211.h
$O/ApplV.o: ApplV.cc \
	Appl.h \
	ApplV.h \
	ExtraClasses.h \
	msg/Beacon_m.h \
	msg/CRL_Piece_m.h \
	msg/SimpleMsg_m.h \
	$(VEINS_PROJ)/src/base/connectionManager/BaseConnectionManager.h \
	$(VEINS_PROJ)/src/base/connectionManager/NicEntry.h \
	$(VEINS_PROJ)/src/base/messages/MacPkt_m.h \
	$(VEINS_PROJ)/src/base/modules/BaseBattery.h \
	$(VEINS_PROJ)/src/base/modules/BaseLayer.h \
	$(VEINS_PROJ)/src/base/modules/BaseMacLayer.h \
	$(VEINS_PROJ)/src/base/modules/BaseMobility.h \
	$(VEINS_PROJ)/src/base/modules/BaseModule.h \
	$(VEINS_PROJ)/src/base/modules/BaseWorldUtility.h \
	$(VEINS_PROJ)/src/base/modules/BatteryAccess.h \
	$(VEINS_PROJ)/src/base/phyLayer/Interpolation.h \
	$(VEINS_PROJ)/src/base/phyLayer/MappingBase.h \
	$(VEINS_PROJ)/src/base/utils/Coord.h \
	$(VEINS_PROJ)/src/base/utils/FWMath.h \
	$(VEINS_PROJ)/src/base/utils/FindModule.h \
	$(VEINS_PROJ)/src/base/utils/HostState.h \
	$(VEINS_PROJ)/src/base/utils/MiXiMDefs.h \
	$(VEINS_PROJ)/src/base/utils/Move.h \
	$(VEINS_PROJ)/src/base/utils/NetwControlInfo.h \
	$(VEINS_PROJ)/src/base/utils/PassedMessage.h \
	$(VEINS_PROJ)/src/base/utils/SimpleAddress.h \
	$(VEINS_PROJ)/src/base/utils/miximkerneldefs.h \
	$(VEINS_PROJ)/src/modules/mac/Mac80211.h \
	$(VEINS_PROJ)/src/modules/mac/Mac80211MultiChannel.h \
	$(VEINS_PROJ)/src/modules/messages/Mac80211Pkt_m.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIColor.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIMobility.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIScenarioManager.h \
	$(VEINS_PROJ)/src/modules/obstacle/Obstacle.h \
	$(VEINS_PROJ)/src/modules/obstacle/ObstacleControl.h \
	$(VEINS_PROJ)/src/modules/utility/Consts80211.h \
	$(VEINS_PROJ)/src/modules/world/annotations/AnnotationManager.h
$O/ApplV_BBroadcast.o: ApplV_BBroadcast.cc \
	Appl.h \
	ApplV.h \
	ApplV_BBroadcast.h \
	ExtraClasses.h \
	msg/Beacon_m.h \
	msg/CRL_Piece_m.h \
	msg/SimpleMsg_m.h \
	$(VEINS_PROJ)/src/base/connectionManager/BaseConnectionManager.h \
	$(VEINS_PROJ)/src/base/connectionManager/NicEntry.h \
	$(VEINS_PROJ)/src/base/messages/MacPkt_m.h \
	$(VEINS_PROJ)/src/base/modules/BaseBattery.h \
	$(VEINS_PROJ)/src/base/modules/BaseLayer.h \
	$(VEINS_PROJ)/src/base/modules/BaseMacLayer.h \
	$(VEINS_PROJ)/src/base/modules/BaseMobility.h \
	$(VEINS_PROJ)/src/base/modules/BaseModule.h \
	$(VEINS_PROJ)/src/base/modules/BaseWorldUtility.h \
	$(VEINS_PROJ)/src/base/modules/BatteryAccess.h \
	$(VEINS_PROJ)/src/base/phyLayer/Interpolation.h \
	$(VEINS_PROJ)/src/base/phyLayer/MappingBase.h \
	$(VEINS_PROJ)/src/base/utils/Coord.h \
	$(VEINS_PROJ)/src/base/utils/FWMath.h \
	$(VEINS_PROJ)/src/base/utils/FindModule.h \
	$(VEINS_PROJ)/src/base/utils/HostState.h \
	$(VEINS_PROJ)/src/base/utils/MiXiMDefs.h \
	$(VEINS_PROJ)/src/base/utils/Move.h \
	$(VEINS_PROJ)/src/base/utils/NetwControlInfo.h \
	$(VEINS_PROJ)/src/base/utils/PassedMessage.h \
	$(VEINS_PROJ)/src/base/utils/SimpleAddress.h \
	$(VEINS_PROJ)/src/base/utils/miximkerneldefs.h \
	$(VEINS_PROJ)/src/modules/mac/Mac80211.h \
	$(VEINS_PROJ)/src/modules/mac/Mac80211MultiChannel.h \
	$(VEINS_PROJ)/src/modules/messages/Mac80211Pkt_m.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIColor.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIMobility.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIScenarioManager.h \
	$(VEINS_PROJ)/src/modules/obstacle/Obstacle.h \
	$(VEINS_PROJ)/src/modules/obstacle/ObstacleControl.h \
	$(VEINS_PROJ)/src/modules/utility/Consts80211.h \
	$(VEINS_PROJ)/src/modules/world/annotations/AnnotationManager.h
$O/ApplV_Broadcast.o: ApplV_Broadcast.cc \
	Appl.h \
	ApplV.h \
	ApplV_Broadcast.h \
	ExtraClasses.h \
	msg/Beacon_m.h \
	msg/CRL_Piece_m.h \
	msg/SimpleMsg_m.h \
	$(VEINS_PROJ)/src/base/connectionManager/BaseConnectionManager.h \
	$(VEINS_PROJ)/src/base/connectionManager/NicEntry.h \
	$(VEINS_PROJ)/src/base/messages/MacPkt_m.h \
	$(VEINS_PROJ)/src/base/modules/BaseBattery.h \
	$(VEINS_PROJ)/src/base/modules/BaseLayer.h \
	$(VEINS_PROJ)/src/base/modules/BaseMacLayer.h \
	$(VEINS_PROJ)/src/base/modules/BaseMobility.h \
	$(VEINS_PROJ)/src/base/modules/BaseModule.h \
	$(VEINS_PROJ)/src/base/modules/BaseWorldUtility.h \
	$(VEINS_PROJ)/src/base/modules/BatteryAccess.h \
	$(VEINS_PROJ)/src/base/phyLayer/Interpolation.h \
	$(VEINS_PROJ)/src/base/phyLayer/MappingBase.h \
	$(VEINS_PROJ)/src/base/utils/Coord.h \
	$(VEINS_PROJ)/src/base/utils/FWMath.h \
	$(VEINS_PROJ)/src/base/utils/FindModule.h \
	$(VEINS_PROJ)/src/base/utils/HostState.h \
	$(VEINS_PROJ)/src/base/utils/MiXiMDefs.h \
	$(VEINS_PROJ)/src/base/utils/Move.h \
	$(VEINS_PROJ)/src/base/utils/NetwControlInfo.h \
	$(VEINS_PROJ)/src/base/utils/PassedMessage.h \
	$(VEINS_PROJ)/src/base/utils/SimpleAddress.h \
	$(VEINS_PROJ)/src/base/utils/miximkerneldefs.h \
	$(VEINS_PROJ)/src/modules/mac/Mac80211.h \
	$(VEINS_PROJ)/src/modules/mac/Mac80211MultiChannel.h \
	$(VEINS_PROJ)/src/modules/messages/Mac80211Pkt_m.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIColor.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIMobility.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIScenarioManager.h \
	$(VEINS_PROJ)/src/modules/obstacle/Obstacle.h \
	$(VEINS_PROJ)/src/modules/obstacle/ObstacleControl.h \
	$(VEINS_PROJ)/src/modules/utility/Consts80211.h \
	$(VEINS_PROJ)/src/modules/world/annotations/AnnotationManager.h
$O/ApplV_C2C_Epidemic.o: ApplV_C2C_Epidemic.cc \
	Appl.h \
	ApplV.h \
	ApplV_C2C_Epidemic.h \
	ExtraClasses.h \
	msg/Beacon_m.h \
	msg/CRL_Piece_m.h \
	msg/Piece_list_m.h \
	msg/SimpleMsg_m.h \
	$(VEINS_PROJ)/src/base/connectionManager/BaseConnectionManager.h \
	$(VEINS_PROJ)/src/base/connectionManager/NicEntry.h \
	$(VEINS_PROJ)/src/base/messages/MacPkt_m.h \
	$(VEINS_PROJ)/src/base/modules/BaseBattery.h \
	$(VEINS_PROJ)/src/base/modules/BaseLayer.h \
	$(VEINS_PROJ)/src/base/modules/BaseMacLayer.h \
	$(VEINS_PROJ)/src/base/modules/BaseMobility.h \
	$(VEINS_PROJ)/src/base/modules/BaseModule.h \
	$(VEINS_PROJ)/src/base/modules/BaseWorldUtility.h \
	$(VEINS_PROJ)/src/base/modules/BatteryAccess.h \
	$(VEINS_PROJ)/src/base/phyLayer/Interpolation.h \
	$(VEINS_PROJ)/src/base/phyLayer/MappingBase.h \
	$(VEINS_PROJ)/src/base/utils/Coord.h \
	$(VEINS_PROJ)/src/base/utils/FWMath.h \
	$(VEINS_PROJ)/src/base/utils/FindModule.h \
	$(VEINS_PROJ)/src/base/utils/HostState.h \
	$(VEINS_PROJ)/src/base/utils/MiXiMDefs.h \
	$(VEINS_PROJ)/src/base/utils/Move.h \
	$(VEINS_PROJ)/src/base/utils/NetwControlInfo.h \
	$(VEINS_PROJ)/src/base/utils/PassedMessage.h \
	$(VEINS_PROJ)/src/base/utils/SimpleAddress.h \
	$(VEINS_PROJ)/src/base/utils/miximkerneldefs.h \
	$(VEINS_PROJ)/src/modules/mac/Mac80211.h \
	$(VEINS_PROJ)/src/modules/mac/Mac80211MultiChannel.h \
	$(VEINS_PROJ)/src/modules/messages/Mac80211Pkt_m.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIColor.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIMobility.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIScenarioManager.h \
	$(VEINS_PROJ)/src/modules/obstacle/Obstacle.h \
	$(VEINS_PROJ)/src/modules/obstacle/ObstacleControl.h \
	$(VEINS_PROJ)/src/modules/utility/Consts80211.h \
	$(VEINS_PROJ)/src/modules/world/annotations/AnnotationManager.h
$O/ApplV_C2C_Epidemic_Ideal.o: ApplV_C2C_Epidemic_Ideal.cc \
	Appl.h \
	ApplV.h \
	ApplV_C2C_Epidemic_Ideal.h \
	ExtraClasses.h \
	msg/Beacon_m.h \
	msg/CRL_Piece_m.h \
	msg/SimpleMsg_m.h \
	$(VEINS_PROJ)/src/base/connectionManager/BaseConnectionManager.h \
	$(VEINS_PROJ)/src/base/connectionManager/NicEntry.h \
	$(VEINS_PROJ)/src/base/messages/MacPkt_m.h \
	$(VEINS_PROJ)/src/base/modules/BaseBattery.h \
	$(VEINS_PROJ)/src/base/modules/BaseLayer.h \
	$(VEINS_PROJ)/src/base/modules/BaseMacLayer.h \
	$(VEINS_PROJ)/src/base/modules/BaseMobility.h \
	$(VEINS_PROJ)/src/base/modules/BaseModule.h \
	$(VEINS_PROJ)/src/base/modules/BaseWorldUtility.h \
	$(VEINS_PROJ)/src/base/modules/BatteryAccess.h \
	$(VEINS_PROJ)/src/base/phyLayer/Interpolation.h \
	$(VEINS_PROJ)/src/base/phyLayer/MappingBase.h \
	$(VEINS_PROJ)/src/base/utils/Coord.h \
	$(VEINS_PROJ)/src/base/utils/FWMath.h \
	$(VEINS_PROJ)/src/base/utils/FindModule.h \
	$(VEINS_PROJ)/src/base/utils/HostState.h \
	$(VEINS_PROJ)/src/base/utils/MiXiMDefs.h \
	$(VEINS_PROJ)/src/base/utils/Move.h \
	$(VEINS_PROJ)/src/base/utils/NetwControlInfo.h \
	$(VEINS_PROJ)/src/base/utils/PassedMessage.h \
	$(VEINS_PROJ)/src/base/utils/SimpleAddress.h \
	$(VEINS_PROJ)/src/base/utils/miximkerneldefs.h \
	$(VEINS_PROJ)/src/modules/mac/Mac80211.h \
	$(VEINS_PROJ)/src/modules/mac/Mac80211MultiChannel.h \
	$(VEINS_PROJ)/src/modules/messages/Mac80211Pkt_m.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIColor.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIMobility.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIScenarioManager.h \
	$(VEINS_PROJ)/src/modules/obstacle/Obstacle.h \
	$(VEINS_PROJ)/src/modules/obstacle/ObstacleControl.h \
	$(VEINS_PROJ)/src/modules/utility/Consts80211.h \
	$(VEINS_PROJ)/src/modules/world/annotations/AnnotationManager.h
$O/ApplV_ICE.o: ApplV_ICE.cc \
	Appl.h \
	ApplV.h \
	ApplV_ICE.h \
	ExtraClasses.h \
	msg/Beacon_m.h \
	msg/CRL_Piece_m.h \
	msg/SimpleMsg_m.h \
	$(VEINS_PROJ)/src/base/connectionManager/BaseConnectionManager.h \
	$(VEINS_PROJ)/src/base/connectionManager/NicEntry.h \
	$(VEINS_PROJ)/src/base/messages/MacPkt_m.h \
	$(VEINS_PROJ)/src/base/modules/BaseBattery.h \
	$(VEINS_PROJ)/src/base/modules/BaseLayer.h \
	$(VEINS_PROJ)/src/base/modules/BaseMacLayer.h \
	$(VEINS_PROJ)/src/base/modules/BaseMobility.h \
	$(VEINS_PROJ)/src/base/modules/BaseModule.h \
	$(VEINS_PROJ)/src/base/modules/BaseWorldUtility.h \
	$(VEINS_PROJ)/src/base/modules/BatteryAccess.h \
	$(VEINS_PROJ)/src/base/phyLayer/Interpolation.h \
	$(VEINS_PROJ)/src/base/phyLayer/MappingBase.h \
	$(VEINS_PROJ)/src/base/utils/Coord.h \
	$(VEINS_PROJ)/src/base/utils/FWMath.h \
	$(VEINS_PROJ)/src/base/utils/FindModule.h \
	$(VEINS_PROJ)/src/base/utils/HostState.h \
	$(VEINS_PROJ)/src/base/utils/MiXiMDefs.h \
	$(VEINS_PROJ)/src/base/utils/Move.h \
	$(VEINS_PROJ)/src/base/utils/NetwControlInfo.h \
	$(VEINS_PROJ)/src/base/utils/PassedMessage.h \
	$(VEINS_PROJ)/src/base/utils/SimpleAddress.h \
	$(VEINS_PROJ)/src/base/utils/miximkerneldefs.h \
	$(VEINS_PROJ)/src/modules/mac/Mac80211.h \
	$(VEINS_PROJ)/src/modules/mac/Mac80211MultiChannel.h \
	$(VEINS_PROJ)/src/modules/messages/Mac80211Pkt_m.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIColor.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIMobility.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIScenarioManager.h \
	$(VEINS_PROJ)/src/modules/obstacle/Obstacle.h \
	$(VEINS_PROJ)/src/modules/obstacle/ObstacleControl.h \
	$(VEINS_PROJ)/src/modules/utility/Consts80211.h \
	$(VEINS_PROJ)/src/modules/world/annotations/AnnotationManager.h
$O/ApplV_ICEv2.o: ApplV_ICEv2.cc \
	Appl.h \
	ApplV.h \
	ApplV_ICEv2.h \
	ExtraClasses.h \
	msg/Beacon_m.h \
	msg/CRL_Piece_m.h \
	msg/SimpleMsg_m.h \
	$(VEINS_PROJ)/src/base/connectionManager/BaseConnectionManager.h \
	$(VEINS_PROJ)/src/base/connectionManager/NicEntry.h \
	$(VEINS_PROJ)/src/base/messages/MacPkt_m.h \
	$(VEINS_PROJ)/src/base/modules/BaseBattery.h \
	$(VEINS_PROJ)/src/base/modules/BaseLayer.h \
	$(VEINS_PROJ)/src/base/modules/BaseMacLayer.h \
	$(VEINS_PROJ)/src/base/modules/BaseMobility.h \
	$(VEINS_PROJ)/src/base/modules/BaseModule.h \
	$(VEINS_PROJ)/src/base/modules/BaseWorldUtility.h \
	$(VEINS_PROJ)/src/base/modules/BatteryAccess.h \
	$(VEINS_PROJ)/src/base/phyLayer/Interpolation.h \
	$(VEINS_PROJ)/src/base/phyLayer/MappingBase.h \
	$(VEINS_PROJ)/src/base/utils/Coord.h \
	$(VEINS_PROJ)/src/base/utils/FWMath.h \
	$(VEINS_PROJ)/src/base/utils/FindModule.h \
	$(VEINS_PROJ)/src/base/utils/HostState.h \
	$(VEINS_PROJ)/src/base/utils/MiXiMDefs.h \
	$(VEINS_PROJ)/src/base/utils/Move.h \
	$(VEINS_PROJ)/src/base/utils/NetwControlInfo.h \
	$(VEINS_PROJ)/src/base/utils/PassedMessage.h \
	$(VEINS_PROJ)/src/base/utils/SimpleAddress.h \
	$(VEINS_PROJ)/src/base/utils/miximkerneldefs.h \
	$(VEINS_PROJ)/src/modules/mac/Mac80211.h \
	$(VEINS_PROJ)/src/modules/mac/Mac80211MultiChannel.h \
	$(VEINS_PROJ)/src/modules/messages/Mac80211Pkt_m.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIColor.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIMobility.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIScenarioManager.h \
	$(VEINS_PROJ)/src/modules/obstacle/Obstacle.h \
	$(VEINS_PROJ)/src/modules/obstacle/ObstacleControl.h \
	$(VEINS_PROJ)/src/modules/utility/Consts80211.h \
	$(VEINS_PROJ)/src/modules/world/annotations/AnnotationManager.h
$O/ApplV_MPB.o: ApplV_MPB.cc \
	Appl.h \
	ApplV.h \
	ApplV_MPB.h \
	ExtraClasses.h \
	msg/Beacon_m.h \
	msg/CRL_Piece_m.h \
	msg/SimpleMsg_m.h \
	$(VEINS_PROJ)/src/base/connectionManager/BaseConnectionManager.h \
	$(VEINS_PROJ)/src/base/connectionManager/NicEntry.h \
	$(VEINS_PROJ)/src/base/messages/MacPkt_m.h \
	$(VEINS_PROJ)/src/base/modules/BaseBattery.h \
	$(VEINS_PROJ)/src/base/modules/BaseLayer.h \
	$(VEINS_PROJ)/src/base/modules/BaseMacLayer.h \
	$(VEINS_PROJ)/src/base/modules/BaseMobility.h \
	$(VEINS_PROJ)/src/base/modules/BaseModule.h \
	$(VEINS_PROJ)/src/base/modules/BaseWorldUtility.h \
	$(VEINS_PROJ)/src/base/modules/BatteryAccess.h \
	$(VEINS_PROJ)/src/base/phyLayer/Interpolation.h \
	$(VEINS_PROJ)/src/base/phyLayer/MappingBase.h \
	$(VEINS_PROJ)/src/base/utils/Coord.h \
	$(VEINS_PROJ)/src/base/utils/FWMath.h \
	$(VEINS_PROJ)/src/base/utils/FindModule.h \
	$(VEINS_PROJ)/src/base/utils/HostState.h \
	$(VEINS_PROJ)/src/base/utils/MiXiMDefs.h \
	$(VEINS_PROJ)/src/base/utils/Move.h \
	$(VEINS_PROJ)/src/base/utils/NetwControlInfo.h \
	$(VEINS_PROJ)/src/base/utils/PassedMessage.h \
	$(VEINS_PROJ)/src/base/utils/SimpleAddress.h \
	$(VEINS_PROJ)/src/base/utils/miximkerneldefs.h \
	$(VEINS_PROJ)/src/modules/mac/Mac80211.h \
	$(VEINS_PROJ)/src/modules/mac/Mac80211MultiChannel.h \
	$(VEINS_PROJ)/src/modules/messages/Mac80211Pkt_m.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIColor.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIMobility.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIScenarioManager.h \
	$(VEINS_PROJ)/src/modules/obstacle/Obstacle.h \
	$(VEINS_PROJ)/src/modules/obstacle/ObstacleControl.h \
	$(VEINS_PROJ)/src/modules/utility/Consts80211.h \
	$(VEINS_PROJ)/src/modules/world/annotations/AnnotationManager.h
$O/ApplV_NewMPB.o: ApplV_NewMPB.cc \
	Appl.h \
	ApplV.h \
	ApplV_NewMPB.h \
	ExtraClasses.h \
	msg/Beacon_m.h \
	msg/CRL_Piece_m.h \
	msg/SimpleMsg_m.h \
	$(VEINS_PROJ)/src/base/connectionManager/BaseConnectionManager.h \
	$(VEINS_PROJ)/src/base/connectionManager/NicEntry.h \
	$(VEINS_PROJ)/src/base/messages/MacPkt_m.h \
	$(VEINS_PROJ)/src/base/modules/BaseBattery.h \
	$(VEINS_PROJ)/src/base/modules/BaseLayer.h \
	$(VEINS_PROJ)/src/base/modules/BaseMacLayer.h \
	$(VEINS_PROJ)/src/base/modules/BaseMobility.h \
	$(VEINS_PROJ)/src/base/modules/BaseModule.h \
	$(VEINS_PROJ)/src/base/modules/BaseWorldUtility.h \
	$(VEINS_PROJ)/src/base/modules/BatteryAccess.h \
	$(VEINS_PROJ)/src/base/phyLayer/Interpolation.h \
	$(VEINS_PROJ)/src/base/phyLayer/MappingBase.h \
	$(VEINS_PROJ)/src/base/utils/Coord.h \
	$(VEINS_PROJ)/src/base/utils/FWMath.h \
	$(VEINS_PROJ)/src/base/utils/FindModule.h \
	$(VEINS_PROJ)/src/base/utils/HostState.h \
	$(VEINS_PROJ)/src/base/utils/MiXiMDefs.h \
	$(VEINS_PROJ)/src/base/utils/Move.h \
	$(VEINS_PROJ)/src/base/utils/NetwControlInfo.h \
	$(VEINS_PROJ)/src/base/utils/PassedMessage.h \
	$(VEINS_PROJ)/src/base/utils/SimpleAddress.h \
	$(VEINS_PROJ)/src/base/utils/miximkerneldefs.h \
	$(VEINS_PROJ)/src/modules/mac/Mac80211.h \
	$(VEINS_PROJ)/src/modules/mac/Mac80211MultiChannel.h \
	$(VEINS_PROJ)/src/modules/messages/Mac80211Pkt_m.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIColor.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIMobility.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIScenarioManager.h \
	$(VEINS_PROJ)/src/modules/obstacle/Obstacle.h \
	$(VEINS_PROJ)/src/modules/obstacle/ObstacleControl.h \
	$(VEINS_PROJ)/src/modules/utility/Consts80211.h \
	$(VEINS_PROJ)/src/modules/world/annotations/AnnotationManager.h
$O/RSUmobility.o: RSUmobility.cc \
	RSUmobility.h \
	$(VEINS_PROJ)/src/base/connectionManager/BaseConnectionManager.h \
	$(VEINS_PROJ)/src/base/connectionManager/NicEntry.h \
	$(VEINS_PROJ)/src/base/modules/BaseBattery.h \
	$(VEINS_PROJ)/src/base/modules/BaseMobility.h \
	$(VEINS_PROJ)/src/base/modules/BaseModule.h \
	$(VEINS_PROJ)/src/base/modules/BaseWorldUtility.h \
	$(VEINS_PROJ)/src/base/modules/BatteryAccess.h \
	$(VEINS_PROJ)/src/base/utils/Coord.h \
	$(VEINS_PROJ)/src/base/utils/FWMath.h \
	$(VEINS_PROJ)/src/base/utils/FindModule.h \
	$(VEINS_PROJ)/src/base/utils/HostState.h \
	$(VEINS_PROJ)/src/base/utils/MiXiMDefs.h \
	$(VEINS_PROJ)/src/base/utils/Move.h \
	$(VEINS_PROJ)/src/base/utils/miximkerneldefs.h \
	$(VEINS_PROJ)/src/modules/mobility/ConstSpeedMobility.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIColor.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIMobility.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIScenarioManager.h \
	$(VEINS_PROJ)/src/modules/obstacle/Obstacle.h \
	$(VEINS_PROJ)/src/modules/obstacle/ObstacleControl.h \
	$(VEINS_PROJ)/src/modules/world/annotations/AnnotationManager.h
$O/Statistics.o: Statistics.cc \
	Appl.h \
	ApplV.h \
	ExtraClasses.h \
	Statistics.h \
	msg/Beacon_m.h \
	msg/CRL_Piece_m.h \
	msg/SimpleMsg_m.h \
	$(VEINS_PROJ)/src/base/connectionManager/BaseConnectionManager.h \
	$(VEINS_PROJ)/src/base/connectionManager/NicEntry.h \
	$(VEINS_PROJ)/src/base/messages/MacPkt_m.h \
	$(VEINS_PROJ)/src/base/modules/BaseBattery.h \
	$(VEINS_PROJ)/src/base/modules/BaseLayer.h \
	$(VEINS_PROJ)/src/base/modules/BaseMacLayer.h \
	$(VEINS_PROJ)/src/base/modules/BaseMobility.h \
	$(VEINS_PROJ)/src/base/modules/BaseModule.h \
	$(VEINS_PROJ)/src/base/modules/BaseWorldUtility.h \
	$(VEINS_PROJ)/src/base/modules/BatteryAccess.h \
	$(VEINS_PROJ)/src/base/phyLayer/Interpolation.h \
	$(VEINS_PROJ)/src/base/phyLayer/MappingBase.h \
	$(VEINS_PROJ)/src/base/utils/Coord.h \
	$(VEINS_PROJ)/src/base/utils/FWMath.h \
	$(VEINS_PROJ)/src/base/utils/FindModule.h \
	$(VEINS_PROJ)/src/base/utils/HostState.h \
	$(VEINS_PROJ)/src/base/utils/MiXiMDefs.h \
	$(VEINS_PROJ)/src/base/utils/Move.h \
	$(VEINS_PROJ)/src/base/utils/NetwControlInfo.h \
	$(VEINS_PROJ)/src/base/utils/PassedMessage.h \
	$(VEINS_PROJ)/src/base/utils/SimpleAddress.h \
	$(VEINS_PROJ)/src/base/utils/miximkerneldefs.h \
	$(VEINS_PROJ)/src/modules/mac/Mac80211.h \
	$(VEINS_PROJ)/src/modules/mac/Mac80211MultiChannel.h \
	$(VEINS_PROJ)/src/modules/messages/Mac80211Pkt_m.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIColor.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIMobility.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIScenarioManager.h \
	$(VEINS_PROJ)/src/modules/obstacle/Obstacle.h \
	$(VEINS_PROJ)/src/modules/obstacle/ObstacleControl.h \
	$(VEINS_PROJ)/src/modules/utility/Consts80211.h \
	$(VEINS_PROJ)/src/modules/world/annotations/AnnotationManager.h
$O/TraCI.o: TraCI.cc \
	TraCI.h \
	$(VEINS_PROJ)/src/base/connectionManager/BaseConnectionManager.h \
	$(VEINS_PROJ)/src/base/connectionManager/NicEntry.h \
	$(VEINS_PROJ)/src/base/modules/BaseWorldUtility.h \
	$(VEINS_PROJ)/src/base/utils/Coord.h \
	$(VEINS_PROJ)/src/base/utils/FWMath.h \
	$(VEINS_PROJ)/src/base/utils/FindModule.h \
	$(VEINS_PROJ)/src/base/utils/MiXiMDefs.h \
	$(VEINS_PROJ)/src/base/utils/miximkerneldefs.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIColor.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIScenarioManager.h \
	$(VEINS_PROJ)/src/modules/mobility/traci/TraCIScenarioManagerLaunchd.h \
	$(VEINS_PROJ)/src/modules/obstacle/Obstacle.h \
	$(VEINS_PROJ)/src/modules/obstacle/ObstacleControl.h \
	$(VEINS_PROJ)/src/modules/world/annotations/AnnotationManager.h

