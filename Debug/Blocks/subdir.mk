################################################################################
# Automatically-generated file. Do not edit!
################################################################################

S_UPPER_SRCS += \
${addprefix $(ROOT)/Blocks/, \
}

CPP_SRCS += \
${addprefix $(ROOT)/Blocks/, \
Amplifier.cpp \
CBlock.cpp \
CConnector.cpp \
Generator.cpp \
}

CC_SRCS += \
${addprefix $(ROOT)/Blocks/, \
}

C_SRCS += \
${addprefix $(ROOT)/Blocks/, \
}

C_UPPER_SRCS += \
${addprefix $(ROOT)/Blocks/, \
}

CXX_SRCS += \
${addprefix $(ROOT)/Blocks/, \
}

S_SRCS += \
${addprefix $(ROOT)/Blocks/, \
}

# Each subdirectory must supply rules for building sources it contributes
Blocks/%.o: $(ROOT)/Blocks/%.cpp
	@echo 'Building file: $<'
	@echo g++ -D__WINDOWS_DS__ -I"E:\Adam\Eclipse\RTSound\RTAudio" -I"C:\Program Files\Microsoft DirectX 9.0 SDK (October 2004)\Include" -O0 -g3 -Wall -c -fmessage-length=0 -o $@ $<
	@g++ -D__WINDOWS_DS__ -I"E:\Adam\Eclipse\RTSound\RTAudio" -I"C:\Program Files\Microsoft DirectX 9.0 SDK (October 2004)\Include" -O0 -g3 -Wall -c -fmessage-length=0 -o $@ $< && \
	echo -n $(@:%.o=%.d) 'Blocks/' > $(@:%.o=%.d) && \
	g++ -MM -MG -P -w -D__WINDOWS_DS__ -I"E:\Adam\Eclipse\RTSound\RTAudio" -I"C:\Program Files\Microsoft DirectX 9.0 SDK (October 2004)\Include" -O0 -g3 -Wall -c -fmessage-length=0  $< >> $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


