##############################################################
			#	BUILD EVOLAB Shared				#
			#	BUILD EVOLAB Project			#
			#	BUILD EVOLAB bdmLsim4 Demo		#
			#	BUILD EVOLAB bdmLsim3 Demo		#
##############################################################
# Type "$> make release" to build the whole stuff in 
# Release mode
##############################################################

config:
	chmod 777 evolab-shared/post-build/post-build-cmd.sh
	cd evolab-shared/$(CONFIG) && make all
	cd evolab-project/$(CONFIG) && make all
	cd evolab-devel/bdmLsim4 && make all
	cd evolab-devel/bdmLsim3 && make all
	cd evolab-devel/DrugRes && make all
config-shared:
	chmod 777 evolab-shared/post-build/post-build-cmd.sh
	cd evolab-shared/$(CONFIG) && make all
debug:
	make config CONFIG=Debug 
release:
	make config CONFIG=Release
release-shared:
	make config-shared CONFIG=Release

clean:
	cd evolab-shared/Release && make clean
	cd evolab-shared/Debug && make clean
	cd evolab-project/Release && make clean
	cd evolab-project/Debug && make clean
	cd evolab-devel/bdmLsim4 && make clean
	cd evolab-devel/bdmLsim3 && make clean
	cd evolab-devel/DrugRes && make clean
clean-shared:
	cd evolab-shared/Release && make clean
	cd evolab-shared/Debug && make clean
clean-project:
	cd evolab-project/Release && make clean
	cd evolab-project/Debug && make clean
clean-devel:
	cd evolab-devel/bdmLsim4 && make clean

.PHONY: debug release config
