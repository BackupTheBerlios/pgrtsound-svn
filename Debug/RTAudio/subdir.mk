################################################################################
# Automatically-generated file. Do not edit!
################################################################################

S_UPPER_SRCS += \
${addprefix $(ROOT)/RTAudio/, \
}

CPP_SRCS += \
${addprefix $(ROOT)/RTAudio/, \
RtAudio.cpp \
}

CC_SRCS += \
${addprefix $(ROOT)/RTAudio/, \
}

C_SRCS += \
${addprefix $(ROOT)/RTAudio/, \
}

C_UPPER_SRCS += \
${addprefix $(ROOT)/RTAudio/, \
}

CXX_SRCS += \
${addprefix $(ROOT)/RTAudio/, \
}

S_SRCS += \
${addprefix $(ROOT)/RTAudio/, \
}

# Each subdirectory must supply rules for building sources it contributes
RTAudio/%.o: $(ROOT)/RTAudio/%.cpp
	@echo 'Building file: $<'
	@echo g++ -D__WINDOWS_DS__ -I"E:\Adam\Eclipse\RTSound\RTAudio" -I"C:\Program Files\Microsoft DirectX 9.0 SDK (October 2004)\Include" -O0 -g3 -Wall -c -fmessage-length=0 -o $@ $<
	@g++ -D__WINDOWS_DS__ -I"E:\Adam\Eclipse\RTSound\RTAudio" -I"C:\Program Files\Microsoft DirectX 9.0 SDK (October 2004)\Include" -O0 -g3 -Wall -c -fmessage-length=0 -o $@ $< && \
	echo -n $(@:%.o=%.d) 'RTAudio/' > $(@:%.o=%.d) && \
	g++ -MM -MG -P -w -D__WINDOWS_DS__ -I"E:\Adam\Eclipse\RTSound\RTAudio" -I"C:\Program Files\Microsoft DirectX 9.0 SDK (October 2004)\Include" -O0 -g3 -Wall -c -fmessage-length=0  $< >> $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '


