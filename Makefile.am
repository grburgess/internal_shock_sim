AUTOMAKE_OPTIONS = subdir-objects foreign
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}

AM_CPPFLAGS =  -I$(top_srcdir)/include



bin_PROGRAMS = grbSim
grbSim_SOURCES =	src/jet.cxx \
			src/shell.cxx  \
			src/radiationProp.cxx  \
			src/radiator.cxx  \
			src/electrons.cxx\
			src/synchrotronElectrons.cxx\
			src/spectrum.cxx\
			src/photons.cxx\
			include/jet.hh    \
			include/shell.hh    \
			include/collisionHist.hh\
			include/shellState.hh\
			include/radiationProp.hh\
			include/radiator.hh\
			include/electrons.hh\
			include/synchrotronElectrons.hh\
			include/spectrum.hh\
			include/photons.hh\
			grbSim.cxx

AM_CPPFLAGS += $(gsl_CFLAGS)

AM_CPPFLAGS += -O2

grbSim_LDADD = $(gsl_LIBS)
