#!/bin/sh
#
echo "Generating build information using autoconf"
echo "This may take a while ..."

srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.
cd "$srcdir"

# Regenerate configuration files
found=false
for aclocal in aclocal-1.15 aclocal-1.14 aclocal
do if which $aclocal >/dev/null 2>&1; then $aclocal -I acinclude && found=true; break; fi
done
if test x$found = xfalse; then
    echo "Couldn't find aclocal, aborting"
    exit 1
fi

found=false
for autoconf in autoconf autoconf259 autoconf-2.59
do if which $autoconf >/dev/null 2>&1; then $autoconf && found=true; break; fi
done
if test x$found = xfalse; then
    echo "Couldn't find autoconf, aborting"
    exit 1
fi
(cd test; sh autogen.sh; rm -rf autom4te.cache)

libtoolize -i -c

rm -rf autom4te.cache

# Run configure for this platform
echo "Now you are ready to run ./configure"
