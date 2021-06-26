



[Some Julia sets by Michael Becker, 8/2003](https://web.archive.org/web/20161024132306/http://www.ijon.de/mathe/julia/index.html)


 original images

![](./images/z6-z.png "description")   

![](./images/z7-z.png "description")   

![](./images/z10-z.png "description")   

![](./images/z14-z.png "description")   

![](./images/z15-z.png "description")   


[code](./src/2005) under GNU GPL licence



See also : 
* [b036.png - original image](https://web.archive.org/web/20161024194536im_/http://www.ijon.de/mathe/julia/sets/b036.png) and [the same image with the src code in commons  ](https://commons.wikimedia.org/wiki/File:Julia_set_f(z)%3D1_over_az5%2Bz3%2Bbz.png)
* [b011.png original image](https://web.archive.org/web/20161024194536im_/http://www.ijon.de/mathe/julia/sets/b011.png) and [the same image with the src code in commons](https://commons.wikimedia.org/wiki/File:Julia_set_p(z)%3D_z%5E3%2B(1.0149042485835864102%2B0.10183008497976470119i)*z.png)
* [b011.png original image](https://web.archive.org/web/20161024194536im_/http://www.ijon.de/mathe/julia/sets/b011.png) and [zoom of the same image with the src code  in commons](https://commons.wikimedia.org/wiki/File:Julia_set_p(z)%3D_z%5E3%2B(1.0149042485835864102%2B0.10183008497976470119i)*z;_(zoom).png
* [b012.png - original image](https://web.archive.org/web/20161024194536im_/http://www.ijon.de/mathe/julia/sets/b012.png) and [the same image with the src code in commons ](https://commons.wikimedia.org/wiki/File:Julia_set_for_f(z)_%3D_z%5E3_%2Bz*(0.1008317508132964*i_%2B_1.004954206930806).png)



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
