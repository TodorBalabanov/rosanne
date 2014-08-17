################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../aiagent.cpp \
../aisuitlengthsolver.cpp \
../ggcard.cpp \
../ggpanel.cpp \
../gmengine.cpp \
../gmrand.cpp \
../gmutil.cpp \
../rabid.cpp \
../raconfig.cpp \
../radlgabout.cpp \
../radlgprefs.cpp \
../radlgrules.cpp \
../raevents.cpp \
../ragamepanel.cpp \
../rainfo.cpp \
../ramain.cpp \
../raplayer.cpp \
../raupdate.cpp \
../ut_suitlengthsolver.cpp 

OBJS += \
./aiagent.o \
./aisuitlengthsolver.o \
./ggcard.o \
./ggpanel.o \
./gmengine.o \
./gmrand.o \
./gmutil.o \
./rabid.o \
./raconfig.o \
./radlgabout.o \
./radlgprefs.o \
./radlgrules.o \
./raevents.o \
./ragamepanel.o \
./rainfo.o \
./ramain.o \
./raplayer.o \
./raupdate.o \
./ut_suitlengthsolver.o 

CPP_DEPS += \
./aiagent.d \
./aisuitlengthsolver.d \
./ggcard.d \
./ggpanel.d \
./gmengine.d \
./gmrand.d \
./gmutil.d \
./rabid.d \
./raconfig.d \
./radlgabout.d \
./radlgprefs.d \
./radlgrules.d \
./raevents.d \
./ragamepanel.d \
./rainfo.d \
./ramain.d \
./raplayer.d \
./raupdate.d \
./ut_suitlengthsolver.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O2 -g -Wall -c -fmessage-length=0 `wx-config --cxxflags` `wx-config --libs` -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


