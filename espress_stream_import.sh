#!/bin/bash

if [ ${#} -lt 1 ]
then
	orgdir="../espress_stream"
else
	orgdir=${1}
fi

impdirs="include/espress_stream"
impdirs="${impdirs} src"
impdirs="${impdirs} api/include/espress_stream"
impdirs="${impdirs} api/src"

depends="include/espress_stream/config.h"
depends="${depends} include/espress_stream/depend.h"
depends="${depends} include/espress_stream/dgram.h"
depends="${depends} include/espress_stream/tick.h"
depends="${depends} include/espress_stream/rand.h"
depends="${depends} src/espress_stream_dgram.c"
depends="${depends} src/espress_stream_tick.c"
depends="${depends} src/espress_stream_rand.c"
depends="${depends} api/include/espress_stream/api_depend.h"

for f in ${depends}
do
	mv espress_stream/${f} espress_stream/${f}.bak
done

for d in ${impdirs}
do
	find espress_stream/${d} -name '*.h' | xargs -r rm 
	find espress_stream/${d} -name '*.c' | xargs -r rm 
done

for d in ${impdirs}
do
	find ${orgdir}/${d} -name '*.h' | xargs -r cp -t espress_stream/${d}
	find ${orgdir}/${d} -name '*.c' | xargs -r cp -t espress_stream/${d}
done

for f in ${depends}
do
	mv -f espress_stream/${f}.bak espress_stream/${f}
done
