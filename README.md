



[Some Julia sets by Michael Becker, 8/2003](https://web.archive.org/web/20161024132306/http://www.ijon.de/mathe/julia/index.html)


 original images

![](./images/z6-z.png "description")   

![](./images/z7-z.png "description")   

![](./images/z10-z.png "description")   

![](./images/z14-z.png "description")   

![](./images/z15-z.png "description")   


[code](./src/2005) under GNU GPL licence




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
