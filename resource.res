Font font_small
{
 filename=font_small.fnt
}

Font menu_item
{
filename=menu_item.fnt
}

Texture bullets
{
	filename=bullets.png
	mipmap=true
}

Texture beams
{
	filename=beams.png
	mipmap=true
}

Sprite map_star
{
	texture=map_star.png
	rect=0,0,32,32
	hotspot=16,16	
}

Texture map_bg
{
	filename=map_bg.png
}

Sprite map_stage
{
	texture=map_stage.png
	rect=0,0,64,64
	hotspot=32,32
}

Sprite map_stageselect
{
	texture=map_stage.png
	rect=64,0,64,64
	hotspot=32,32
}

Sprite menu_item_bg
{
	texture=menu_item_bg.png
	rect=0,0,64,64
	hotspot=32,32
	blendmode=ALPHAADD,COLORMUL,NOZWRITE
}

Sprite scorepoint
{
	texture=scorepoint.png
	rect=0,0,8,8
	hotspot=4,4
}

Sprite menu_bg
{
	texture=bg.png
	rect=0,0,640,480
	mesh=32,32
}

Sprite menu_delta
{
	texture=bg_delta.png
	rect=0,0,256,256
	hotspot=128,128
	color=88FFFFFF
}

Sprite menu_select
{
	texture=bg_delta.png
	rect=0,256,256,32
	hotspot=10,4
	color=88FFFFFF
}

Animation balancedbig
{
	texture=balancedbig.png
	rect=0,0,64,64
	hotspot=32,32
	frames=1
}

Sprite playershotpart
{
	texture=playershotpart.png
	rect=0,0,8,8
	hotspot=4,4
}

Particle playershot
{
	filename=playershot.psi
	sprite=playershotpart
}

Font font_interface
{
 filename=font_interface.fnt
}


Sprite powersmall
{
	texture=power.png
	rect=0,0,9,9
	hotspot=5,5
}

Sprite powerlarge
{
	texture=power.png
	rect=9,0,16,16
	hotspot=8,8
}

Sprite deltabg
{
	texture=deltabg.png
	rect=0,0,640,480
}

Sprite boss1bg
{
	texture=boss1bg.png
	rect=0,0,512,512
	hotspot=256,256
}

Sprite boss1bgspin
{
	texture=boss1bgspin.png
	rect=0,0,512,512
	hotspot=256,256
}

Sprite deltabgscroll
{
	texture=deltabgscroll.png
	rect=0,0,512,512
}

Distortion testbg
{
	texture=deltabgscroll.png
	rect=0,0,512,512
	mesh=32,32
}
 
Animation birth01_1
{
	texture=birth01.png
	rect=64,0,64,64
	hotspot=32,32
	frames=1
}

Animation birth01_2
{
	texture=birth01.png
	rect=0,0,64,64
	hotspot=32,32
	frames=1
}

Animation birth03_1
{
	texture=ringballbirth.png
	rect=0,0,32,32
	hotspot=16,16
	frames=1
}

Animation birth03_2
{
	texture=ringballbirth.png
	rect=32,0,32,32
	hotspot=16,16
	frames=1
}

Animation enemydeath
{
	texture=enemydeath.png
	rect=0,0,128,128
	hotspot=64,64
	frames=1
}
Animation player
{
	texture=player.png
	rect=0,0,20,30
	hotspot=10,15
	frames=1
}

Animation playershot
{
	 texture=playershot.png
	 rect=0,0,20,10
	 hotspot=10,5
	 frames=1
	 color=AAFFFFFF
}

Animation balancedplayerother
{
	 texture=balancedplayerother.png
	 rect=0,0,32,16
	 hotspot=16,8
	 frames=1
	 color=AAFFFFFF
}

Sprite playerhitbox
{
	texture=playerhitbox.png
	rect=0,0,8,8
	hotspot=4,4
}

Animation boss1
{
	texture=boss1.png
	rect=0,0,256,256
	hotspot=128,128
	frames=1
}

Animation en1red
{
	texture=enemies.png
	rect=0,0,32,32
	hotspot=16,16
	frames=1
}
Animation en1green : en1red
{
	rect=32,0,32,32
}
Animation en1blue : en1red
{
	rect=64,0,32,32
}
Animation en1yellow : en1red
{
	rect=96,0,32,32
}
Animation en1cyan : en1red
{
	rect=128,0,32,32
}
Animation en1magenta : en1red
{
	rect=160,0,32,32
}
Animation en1white : en1red
{
	rect=192,0,32,32
}
Animation en1black : en1red
{
	rect=224,0,32,32
}
