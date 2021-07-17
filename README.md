



[Some Julia sets by Michael Becker, 8/2003](https://web.archive.org/web/20161024132306/http://www.ijon.de/mathe/julia/index.html)




#  original images z^n - z

## n is even 
![](./images/z6-z.png "z^6-z")   

![](./images/z10-z.png "z^10-z")   

![](./images/z14-z.png "z^14-z")   

## n is odd 
![](./images/z7-z.png "z^7-z")   

![](./images/z15-z.png "z^15-z")   


# code 
[original code from 2005](./src/2005) under [GNU GPL licence](http://gnugpl.org/)



See also new not original code and images: 
* [b036.png - original image](https://web.archive.org/web/20161024194536im_/http://www.ijon.de/mathe/julia/sets/b036.png) and [the same image with the src code in commons  ](https://commons.wikimedia.org/wiki/File:Julia_set_f(z)%3D1_over_az5%2Bz3%2Bbz.png)
* [b011.png original image](https://web.archive.org/web/20161024194536im_/http://www.ijon.de/mathe/julia/sets/b011.png) and [the same image with the src code in commons](https://commons.wikimedia.org/wiki/File:Julia_set_p(z)%3D_z%5E3%2B(1.0149042485835864102%2B0.10183008497976470119i)*z.png)
* [b011.png original image](https://web.archive.org/web/20161024194536im_/http://www.ijon.de/mathe/julia/sets/b011.png) and [zoom of the same image with the src code  in commons](https://commons.wikimedia.org/wiki/File:Julia_set_p(z)%3D_z%5E3%2B(1.0149042485835864102%2B0.10183008497976470119i)*z;_(zoom).png)
* [b012.png - original image](https://web.archive.org/web/20161024194536im_/http://www.ijon.de/mathe/julia/sets/b012.png) and [the same image with the src code in commons ](https://commons.wikimedia.org/wiki/File:Julia_set_for_f(z)_%3D_z%5E3_%2Bz*(0.1008317508132964*i_%2B_1.004954206930806).png)
* [b015.png - original image](https://web.archive.org/web/20160504150529im_/http://www.ijon.de/mathe/julia/sets/b015.png) and [the same image with the src code in commons ]( https://commons.wikimedia.org/wiki/File:Julia_set_for_f(z)%3D_z%5E14-z.png#%7B%7Bint%3Afiledesc%7D%7D)


# compiling original code

## Ubuntu 20.10
Short description : compilation failed   


```
 make
cc -Wall -g -I /usr/include/glib-1.2 -I /usr/include/gtk-1.2 -I /usr/lib/glib/include  -c complex.c
cc -Wall -g -I /usr/include/glib-1.2 -I /usr/include/gtk-1.2 -I /usr/lib/glib/include  -c imagearea.c
In file included from imagearea.c:12:
imagearea.h:4:10: fatal error: glib.h: No such file or directory
    4 | #include <glib.h>
      |          ^~~~~~~~
compilation terminated.
make: *** [makefile:16: imagearea.o] Error 1
```

Solutio:

Find file glib.h in:
* [packages](https://packages.ubuntu.com/search?suite=bionic&arch=any&mode=exactfilename&searchon=contents&keywords=glib.h)
* disk


Install  -dev package containing glib.h:

```bash
sudo apt-get install libglib2.0-dev
```


## instal glib1.2

```
sh
./configure
creating cache ./config.cache
checking for a BSD compatible install... /usr/bin/install -c
checking whether build environment is sane... yes
checking whether make sets ${MAKE}... yes
checking for working aclocal... found
checking for working autoconf... found
checking for working automake... found
checking for working autoheader... found
checking for working makeinfo... missing
checking host system type... Invalid configuration `x86_64-unknown-linux-gnu': machine `x86_64-unknown' not recognized

checking build system type... Invalid configuration `x86_64-unknown-linux-gnu': machine `x86_64-unknown' not recognized

checking for ranlib... ranlib
checking for gcc... gcc
checking whether the C compiler (gcc  ) works... yes
checking whether the C compiler (gcc  ) is a cross-compiler... no
checking whether we are using GNU C... yes
checking whether gcc accepts -g... yes
checking for ld used by GCC... /usr/bin/ld
checking if the linker (/usr/bin/ld) is GNU ld... yes
checking for BSD-compatible nm... /usr/bin/nm -B
checking command to parse /usr/bin/nm -B output... yes
checking for _ prefix in compiled symbols... no
checking whether ln -s works... yes
ltconfig: you must specify a host type if you use `--no-verify'
Try `ltconfig --help' for more information.
configure: error: libtool configure failed
```

solution: 


  ./configure --build=i386-linux-gnu --host=i386-linux-gnu 
  
  
then 

  make
  
  
``` 
   | ^~~~~~~~~~~~~~~~~
gcc -DHAVE_CONFIG_H -I. -I. -I. -DG_LOG_DOMAIN=g_log_domain_glib -g -O2 -Wall -D_REENTRANT -c gtree.c -o gtree.o >/dev/null 2>&1
/bin/sh ./libtool --mode=compile gcc -DHAVE_CONFIG_H -I. -I. -I. -DG_LOG_DOMAIN=g_log_domain_glib     -g -O2 -Wall  -D_REENTRANT -c gutils.c
gcc -DHAVE_CONFIG_H -I. -I. -I. -DG_LOG_DOMAIN=g_log_domain_glib -g -O2 -Wall -D_REENTRANT -c -fPIC -DPIC gutils.c -o gutils.lo
gcc -DHAVE_CONFIG_H -I. -I. -I. -DG_LOG_DOMAIN=g_log_domain_glib -g -O2 -Wall -D_REENTRANT -c gutils.c -o gutils.o >/dev/null 2>&1
/bin/sh ./libtool --mode=link gcc  -g -O2 -Wall  -D_REENTRANT  -o libglib.la -rpath /usr/local/lib -version-info 0:0:0 	-release 1.2 garray.lo gcache.lo gcompletion.lo gdataset.lo gdate.lo gerror.lo ghash.lo ghook.lo giochannel.lo giounix.lo glist.lo gmain.lo gmem.lo gmessages.lo gmutex.lo gnode.lo gprimes.lo grel.lo gscanner.lo gslist.lo gstrfuncs.lo gstring.lo gtimer.lo gtree.lo gutils.lo  
mkdir .libs

*** Warning: This library needs some functionality provided by -lc.
*** I have the capability to make that library automatically link in when
*** you link to this library.  But I can only do this if you have a
*** shared version of the library, which you do not appear to have.
```

also failed







# git
```git
git init
git add README.md
git commit -m "first commit"
git branch -M main
git remote add origin https://github.com/adammaj1/Some-Julia-sets-by-Michael-Becker-.git
git push -u origin main
```



## Subdirectory

```git
mkdir images
git add *.png
git mv  *.png ./images
git commit -m "move"
git push -u origin main
```

then link the images:


```
![](./images/n.png "description") 
```


```
gitm mv -f 
```
