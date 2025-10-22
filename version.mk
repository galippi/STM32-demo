ifeq ($(wildcard version.h),)
VERSION_OLD:=(no file)
else
VERSION_OLD := $(strip $(shell cat version.h))
endif

ifeq ($(VERSION_OLD),)
VERSION_OLD:=(none)
endif

GIT_COMMIT_ID := $(shell git rev-parse --short --verify HEAD 2>/dev/null)
MODIFIED := $(shell git status --short 2> /dev/null)
LAST_TAG := $(shell git describe --tags --exact-match 2> /dev/null)

ifeq ($(GIT_COMMIT_ID),)
GIT_COMMIT_ID := (git error)
else
  ifeq ($(MODIFIED),)
    # not modified version
    ifeq ($(LAST_TAG),)
      # no tag - keep GIT_COMMIT_ID
    else
      GIT_COMMIT_ID := $(LAST_TAG)
    endif
  else
    # modified content
    GIT_COMMIT_ID := $(GIT_COMMIT_ID)-modified
  endif
endif

VERSION_FILE = $(wildcard version.h)

VERSION_NEW := \#define GIT_COMMIT_ID "$(GIT_COMMIT_ID)"

#VERSIONFLAGS += -DFILE_SYNC_VERSION=\"$(VERSION_OLD)\" -DFILE_SYNC_GIT=\"$(GIT_COMMIT_ID)\"
VERSIONFLAGS += -DFILE_SYNC_VERSION="$(VERSION_OLD)" -DFILE_SYNC_GIT="$(GIT_COMMIT_ID)" -DVER_NEW="$(VERSION_NEW)"

#CFLAGS   += $(VERSIONFLAGS)
#CPPFLAGS += $(VERSIONFLAGS)

ifneq ($(VERSION_OLD),$(VERSION_NEW))

.PHONY: version.h

#version.h: version_h_debug
version.h:
	@echo Generating $@
	echo '$(VERSION_NEW)' >$@

else

version.h:
	@echo Generating $@ is skipped

endif

.PHONY : version_h_debug
version_h_debug:
	@echo Generating $@
	@echo "VERSION_OLD = $(VERSION_OLD)"
	@echo "VERSION_NEW = $(VERSION_NEW)"
	@echo VERSION_FILE=$(VERSION_FILE)
