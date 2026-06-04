##############################################################
			#	BUILD EEE-MISC				#
##############################################################
# Type "$> make release" to build the whole stuff in 
# Release mode
##############################################################

config:
	cd drugres_stoch_ode/$(CONFIG) && make all
	cd drugres/$(CONFIG) && make all
	cd parvir/$(CONFIG) && make all
debug:
	make config CONFIG=Debug 
release:
	make config CONFIG=Release

clean:
	cd drugres_stoch_ode/Release && make clean
	cd drugres/Release && make clean
	cd parvir/Release && make clean
	cd drugres_stoch_ode/Debug && make clean
	cd drugres/Debug && make clean
	cd parvir/Debug && make clean
clean-stoch-ode:
	cd drugres_stoch_ode/Release && make clean
	cd drugres_stoch_ode/Debug && make clean
clean-drugres:
	cd drugres/Release && make clean
	cd drugres/Debug && make clean
clean-parvir:
	cd parvir/Release && make clean
	cd parvir/Debug && make clean

.PHONY: debug release config
