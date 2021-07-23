



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
* [original code from 2005](./src/2005) under [GNU GPL licence](http://gnugpl.org/)
* [modified code from 2021](./src/2021) under [GNU GPL licence](http://gnugpl.org/)
* see also new not original code and images: 
  * [b036.png - original image](https://web.archive.org/web/20161024194536im_/http://www.ijon.de/mathe/julia/sets/b036.png) and [the same image with the src code in commons  ](https://commons.wikimedia.org/wiki/File:Julia_set_f(z)%3D1_over_az5%2Bz3%2Bbz.png)
  * [b011.png original image](https://web.archive.org/web/20161024194536im_/http://www.ijon.de/mathe/julia/sets/b011.png) and [the same image with the src code in commons](https://commons.wikimedia.org/wiki/File:Julia_set_p(z)%3D_z%5E3%2B(1.0149042485835864102%2B0.10183008497976470119i)*z.png)
  * [b011.png original image](https://web.archive.org/web/20161024194536im_/http://www.ijon.de/mathe/julia/sets/b011.png) and [zoom of the same image with the src code  in commons](https://commons.wikimedia.org/wiki/File:Julia_set_p(z)%3D_z%5E3%2B(1.0149042485835864102%2B0.10183008497976470119i)*z;_(zoom).png)
  * [b012.png - original image](https://web.archive.org/web/20161024194536im_/http://www.ijon.de/mathe/julia/sets/b012.png) and [the same image with the src code in commons ](https://commons.wikimedia.org/wiki/File:Julia_set_for_f(z)_%3D_z%5E3_%2Bz*(0.1008317508132964*i_%2B_1.004954206930806).png)
  * [b015.png - original image](https://web.archive.org/web/20160504150529im_/http://www.ijon.de/mathe/julia/sets/b015.png) and [the same image with the src code in commons ]( https://commons.wikimedia.org/wiki/File:Julia_set_for_f(z)%3D_z%5E14-z.png#%7B%7Bint%3Afiledesc%7D%7D)


## compiling and running on Ubuntu 21.04
* [original code 2005](/doc/INSTALL.md)
* [modified code 2021](./src/2021/)

```
make
/usr/bin/gcc -std=gnu89 -Wall -g -I /usr/include/glib-1.2 -I /usr/include/gtk-1.2 -I /usr/lib/glib/include  -c cpoly.c
/usr/bin/gcc -std=gnu89 -Wall -g -I /usr/include/glib-1.2 -I /usr/include/gtk-1.2 -I /usr/lib/glib/include  -o julia2 complex.o imagearea.o zero.o cpoly.o julia2.o `gtk-config --cflags` `gtk-config --libs` -lgthread

./julia2

Gtk-WARNING **: Failed to load module "libgail.so": libgail.so: cannot open shared object file: No such file or directory

Gtk-WARNING **: Failed to load module "libatk-bridge.so": libatk-bridge.so: cannot open shared object file: No such file or directory
Polynomial P 	+1.000000*z^10	-1.000000*z^1	
Polynomial Q 	+1.000000
Polynomial P1 = diff(P) =  	+10.000000*z^9	-1.000000
Polynomial Q1 = diff(Q) =  	

rational map R = P/Q 
..................................................................................................................................................................................................................................................................................................................zero failed. error code 1 at -1.008677+0.357789*i
.......................................................................................................................................................................................................................................................................................................................................................................................................................................................................................................zero failed. error code -1 at +0.277964-0.977380*i

```


[Save Window as am image to directory Images : Alt-PrtScn](https://askubuntu.com/questions/220225/how-can-i-print-screen-just-one-window-and-not-my-entire-desktop)

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
