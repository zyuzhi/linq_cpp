RM := rm -rf

all: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -O3 -std=c++0x sample.cpp -o sample
	@echo 'Finished building target: $@'
	@echo ' '

clean:
	-$(RM) sample
	-@echo ' '

