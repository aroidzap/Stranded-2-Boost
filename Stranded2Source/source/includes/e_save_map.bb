;############################################ Engine Save Map

;### SAVE MAP
Function save_map(path$,mode$,pw$,s_skills=1,s_items=1,s_vars=1,s_diary=1,s_states=1,s_locks=1)
	Local stream=BufWriteFile(path$)
	If stream=0 Then Return 0
	
	;Event: Presave
	If m_section<>Csection_editor Then
		parse_globalevent("presave")
		parse_all()
	EndIf
	
	;### Headers
	
	;Main Header
	bmpf_loadscreen(s$(10),10)
	BufWriteLine(stream,"### Stranded II Mapfile �by Unreal Software 2004-2007")
	BufWriteLine(stream,Cversion$)					;Version
	BufWriteLine(stream,CurrentDate())				;Date
	BufWriteLine(stream,CurrentTime())				;Time
	BufWriteLine(stream,"default")					;Format
	BufWriteLine(stream,mode$)						;Mode
	
	;Type Format
	Local tfobject=0
	Local tfunit=0
	Local tfitem=0
	If Cobject_count>255 Then tfobject=1
	If Cunit_count>255 Then tfunit=1
	If Citem_count>255 Then tfitem=1
	If (tfobject+tfunit+tfitem)>0 Then
		Local tfstring$=""
		tfstring$=tfstring$+Str(tfobject)
		tfstring$=tfstring$+Str(tfunit)
		tfstring$=tfstring$+Str(tfitem)
		BufWriteLine(stream,tfstring$)
	Else
		BufWriteLine(stream,"")
	EndIf
	
	BufWriteLine(stream,"")
	BufWriteLine(stream,"")
	BufWriteLine(stream,"")
	BufWriteLine(stream,"")
	BufWriteLine(stream,"###")						;###
	
	;Image
	save_map_image(mode$)
	SetBuffer ImageBuffer(map_image)
	LockBuffer ImageBuffer(map_image)
	For x=0 To 96-1
		For y=0 To 72-1
			rgb=ReadPixelFast(x,y)
			BufWriteByte(stream,(rgb And $FF0000)/$10000)	;R
			BufWriteByte(stream,(rgb And $FF00)/$100)		;G
			BufWriteByte(stream,rgb And $FF)				;B
		Next
	Next
	UnlockBuffer ImageBuffer(map_image)
	BufferDirty ImageBuffer(map_image)
	SetBuffer BackBuffer()
	
	;Password Header
	Local pwkey=Rand(5,250)
	BufWriteByte(stream,pwkey)						;Code Key
	encodedpw$=code(pw$,pwkey,0)
	BufWriteLine(stream,encodedpw$)				;Password
	map_password$=pw$
	
	;Map Vars
	BufWriteInt(stream,map_day)							;Map Time Day
	BufWriteByte(stream,map_hour)							;Map Time Hour
	BufWriteByte(stream,map_minute)						;Map Time Minute
	BufWriteByte(stream,map_freezetime)					;Map Time frozen?
	BufWriteString(stream,map_skybox$)						;Map Skybox
	BufWriteByte(stream,map_multiplayer)					;Map Multiplayer
	BufWriteByte(stream,map_climate)						;Map Climate
	BufWriteString(stream,map_music$)						;Map Music
	BufWriteString(stream,map_briefing$)					;Map Briefing
	BufWriteByte(stream,map_fog(0))						;Map Fog R
	BufWriteByte(stream,map_fog(1))						;Map Fog G
	BufWriteByte(stream,map_fog(2))						;Map Fog B
	BufWriteByte(stream,map_fog(3))						;Map Fog Mode
	BufWriteByte(stream,0)									;Extended Stuff
	
	;Interface
	For i=0 To 9
		BufWriteString(stream,in_quickslot(i))				;Quickslot
	Next
	
	
	;### Maps
	
	;Colormap
	bmpf_loadscreen(s$(11),20)
	h=TextureHeight(ter_tex_color)						;Colortexture Size
	BufWriteInt(stream,h)
	SetBuffer TextureBuffer(ter_tex_color)
	LockBuffer TextureBuffer(ter_tex_color)
	For x=0 To h-1
		For y=0 To h-1
			rgb=ReadPixelFast(x,y)
			BufWriteByte(stream,(rgb And $FF0000)/$10000)	;R
			BufWriteByte(stream,(rgb And $FF00)/$100)		;G
			BufWriteByte(stream,rgb And $FF)				;B
		Next
	Next
	UnlockBuffer TextureBuffer(ter_tex_color)
	BufferDirty TextureBuffer(ter_tex_color)
	SetBuffer BackBuffer()
	
	;Heightmap
	bmpf_loadscreen(s$(12),30)
	BufWriteInt(stream,ter_size)							;Terrain Size
	For x=0 To ter_size
		For y=0 To ter_size
			BufWriteFloat(stream,TerrainHeight(ter,x,y))	;Height
		Next
	Next
	
	;Grassmap
	bmpf_loadscreen(s$(13),35)
	For x=0 To h
		For y=0 To h
			BufWriteByte(stream,grass_rgb(x,y,3))			;Gras? 1/0
		Next
	Next
	
		
	;### Stuff
	
	;Object
	bmpf_loadscreen(s$(14),40)
	c=0
	For Tobject.Tobject=Each Tobject:c=c+1:Next
	BufWriteInt(stream,c)													;Count
	For Tobject.Tobject=Each Tobject
		BufWriteInt(stream,Tobject\id)										;ID
		If tfobject Then												;Typ
			BufWriteShort(stream,Tobject\typ)
		Else
			BufWriteByte(stream,Tobject\typ)
		EndIf
		BufWriteFloat(stream,EntityX(Tobject\h))							;X
		BufWriteFloat(stream,EntityZ(Tobject\h))							;Z
		BufWriteFloat(stream,EntityYaw(Tobject\h))							;Yaw
		BufWriteFloat(stream,Tobject\health#)								;Health
		BufWriteFloat(stream,Tobject\health_max#)							;Health Max
		BufWriteInt(stream,Tobject\daytimer)								;Daytimer
	Next

	;Unit
	Local peak=100
	If mode$="sav" Then peak=0
	bmpf_loadscreen(s$(15),70)
	c=0
	For Tunit.Tunit=Each Tunit
		If Tunit\id>=peak Then c=c+1
	Next
	BufWriteInt(stream,c)													;Count
	For Tunit.Tunit=Each Tunit
		If Tunit\id>=peak Then
			If mode$="map" Then											;Cache AI Center
				Tunit\ai_cx#=EntityX(Tunit\h)
				Tunit\ai_cz#=EntityZ(Tunit\h)
			EndIf
			BufWriteInt(stream,Tunit\id)									;ID
			If tfunit Then												;Typ
				BufWriteShort(stream,Tunit\typ)
			Else
				BufWriteByte(stream,Tunit\typ)
			EndIf								
			BufWriteFloat(stream,EntityX(Tunit\h))							;X
			BufWriteFloat(stream,EntityY(Tunit\h))							;Y
			BufWriteFloat(stream,EntityZ(Tunit\h))							;Z
			BufWriteFloat(stream,EntityYaw(Tunit\h))						;Yaw
			BufWriteFloat(stream,Tunit\health#)							;Health
			BufWriteFloat(stream,Tunit\health_max#)						;Health Max
			BufWriteFloat(stream,Tunit\hunger#)							;Hunger
			BufWriteFloat(stream,Tunit\thirst#)							;Thirst
			BufWriteFloat(stream,Tunit\exhaustion#)						;Exhaustion
			BufWriteFloat(stream,Tunit\ai_cx#)								;AI Center X
			BufWriteFloat(stream,Tunit\ai_cz#)								;AI Center Z
		EndIf
	Next
	
	;Item
	bmpf_loadscreen(s$(16),80)
	If s_items=1 Then
		c=0
		For Titem.Titem=Each Titem:c=c+1:Next
		BufWriteInt(stream,c)													;Count
		For Titem.Titem=Each Titem
			BufWriteInt(stream,Titem\id)										;ID
			If tfitem Then													;Typ
				BufWriteShort(stream,Titem\typ)
			Else
				BufWriteByte(stream,Titem\typ)
			EndIf
			BufWriteFloat(stream,EntityX(Titem\h))								;X
			BufWriteFloat(stream,EntityY(Titem\h))								;Y
			BufWriteFloat(stream,EntityZ(Titem\h))								;Z
			BufWriteFloat(stream,EntityYaw(Titem\h))							;Yaw
			BufWriteFloat(stream,Titem\health#)								;Health
			BufWriteInt(stream,Titem\count)									;Count
			BufWriteByte(stream,Titem\parent_class)							;Parent Class
			BufWriteByte(stream,Titem\parent_mode)								;Parent Mode
			BufWriteInt(stream,Titem\parent_id)								;Parent ID
		Next
	Else
		c=0
		For Titem.Titem=Each Titem
			If Titem\parent_class<>Cclass_unit Or Titem\parent_id<>1 Then
				c=c+1
			EndIf
		Next
		BufWriteInt(stream,c)													;Count
		For Titem.Titem=Each Titem
			If Titem\parent_class<>Cclass_unit Or Titem\parent_id<>1 Then
				BufWriteInt(stream,Titem\id)									;ID
				If tfitem Then												;Typ
					BufWriteShort(stream,Titem\typ)
				Else
					BufWriteByte(stream,Titem\typ)
				EndIf
				BufWriteFloat(stream,EntityX(Titem\h))							;X
				BufWriteFloat(stream,EntityY(Titem\h))							;Y
				BufWriteFloat(stream,EntityZ(Titem\h))							;Z
				BufWriteFloat(stream,EntityYaw(Titem\h))						;Yaw
				BufWriteFloat(stream,Titem\health#)							;Health
				BufWriteInt(stream,Titem\count)								;Count
				BufWriteByte(stream,Titem\parent_class)						;Parent Class
				BufWriteByte(stream,Titem\parent_mode)							;Parent Mode
				BufWriteInt(stream,Titem\parent_id)							;Parent ID
			EndIf
		Next		
	EndIf
		
	;Info
	bmpf_loadscreen(s$(17),90)
	c=0
	For Tinfo.Tinfo=Each Tinfo:c=c+1:Next								
	BufWriteInt(stream,c)														;Count
	For Tinfo.Tinfo=Each Tinfo
		BufWriteInt(stream,Tinfo\id)											;ID
		BufWriteByte(stream,Tinfo\typ)											;Typ
		BufWriteFloat(stream,EntityX(Tinfo\h))									;X
		BufWriteFloat(stream,EntityY(Tinfo\h))									;Y
		BufWriteFloat(stream,EntityZ(Tinfo\h))									;Z
		BufWriteFloat(stream,EntityPitch(Tinfo\h))								;Pitch
		BufWriteFloat(stream,EntityYaw(Tinfo\h))								;Yaw
		BufWriteString(stream,Tinfo\vars$)										;Vars
	Next
	
	;State
	bmpf_loadscreen(s$(18),92)
	If s_states=1 Then
		c=0
		For Tstate.Tstate=Each Tstate:c=c+1:Next							
		BufWriteInt(stream,c)													;Count
		For Tstate.Tstate=Each Tstate
			BufWriteByte(stream,Tstate\typ)									;Typ
			BufWriteByte(stream,Tstate\parent_class)							;Parent Class
			BufWriteInt(stream,Tstate\parent_id)								;Parent ID
			BufWriteFloat(stream,Tstate\x#)									;X
			BufWriteFloat(stream,Tstate\y#)									;Y
			BufWriteFloat(stream,Tstate\z#)									;Z
			BufWriteFloat(stream,Tstate\fx#)									;FX
			BufWriteFloat(stream,Tstate\fy#)									;FY
			BufWriteFloat(stream,Tstate\fz#)									;FZ
			BufWriteInt(stream,Tstate\value)									;Value
			BufWriteFloat(stream,Tstate\value_f#)								;Value Float
			BufWriteString(stream,Tstate\value_s$)								;Value String
		Next
	Else
		c=0
		For Tstate.Tstate=Each Tstate
			If Tstate\parent_class<>Cclass_unit Or Tstate\parent_id<>1 Then
				c=c+1
			EndIf
		Next
		BufWriteInt(stream,c)													;Count
		For Tstate.Tstate=Each Tstate
			If Tstate\parent_class<>Cclass_unit Or Tstate\parent_id<>1 Then
				BufWriteByte(stream,Tstate\typ)								;Typ
				BufWriteByte(stream,Tstate\parent_class)						;Parent Class
				BufWriteInt(stream,Tstate\parent_id)							;Parent ID
				BufWriteFloat(stream,Tstate\x#)								;X
				BufWriteFloat(stream,Tstate\y#)								;Y
				BufWriteFloat(stream,Tstate\z#)								;Z
				BufWriteFloat(stream,Tstate\fx#)								;FX
				BufWriteFloat(stream,Tstate\fy#)								;FY
				BufWriteFloat(stream,Tstate\fz#)								;FZ
				BufWriteInt(stream,Tstate\value)								;Value
				BufWriteFloat(stream,Tstate\value_f#)							;Value Float
				BufWriteString(stream,Tstate\value_s$)							;Value String
			EndIf
		Next		
	EndIf
	
	;Extensions
	bmpf_loadscreen(s$(19),95)
	c=0
	save_map_extensions(mode$,s_items)
	;free temp vars - dont save them
	For Tx.Tx=Each Tx
		If Tx\mode=1 Then
			If Tx\stuff$="temp" Then Delete Tx
		EndIf
	Next
	;Save
	If s_skills+s_vars+s_diary+s_locks=4 Then
		For Tx.Tx=Each Tx:c=c+1:Next
		BufWriteInt(stream,c)													;Count
		For Tx.Tx=Each Tx
			BufWriteByte(stream,Tx\typ)										;Typ
			BufWriteByte(stream,Tx\parent_class)								;Parent Class
			BufWriteInt(stream,Tx\parent_id)									;Parent ID
			BufWriteInt(stream,Tx\mode)										;Mode
			If Tx\mode=0 Then												;Key (dont save script keys)
				BufWriteString(stream,"")
			Else
				BufWriteString(stream,Tx\key$)
			EndIf
			BufWriteString(stream,Tx\value$)									;Value
			BufWriteString(stream,Tx\stuff$)									;Stuff
			If Tx\mode=6 Then Delete Tx
		Next
	Else
		For Tx.Tx=Each Tx
			ds=1
			If s_skills=0 And Tx\mode=5 Then ds=0
			If s_vars=0 And Tx\mode=1 Then ds=0
			If s_diary=0 And Tx\mode=2 Then ds=0
			If s_locks=0 And Tx\mode=3 Then ds=0
			If ds=1 Then c=c+1
		Next
		BufWriteInt(stream,c)													;Count
		For Tx.Tx=Each Tx
			ds=1
			If s_skills=0 And Tx\mode=5 Then ds=0
			If s_vars=0 And Tx\mode=1 Then ds=0
			If s_diary=0 And Tx\mode=2 Then ds=0
			If s_locks=0 And Tx\mode=3 Then ds=0
			If ds=1 Then
				BufWriteByte(stream,Tx\typ)									;Typ
				BufWriteByte(stream,Tx\parent_class)							;Parent Class
				BufWriteInt(stream,Tx\parent_id)								;Parent ID
				BufWriteInt(stream,Tx\mode)									;Mode
				If Tx\mode=0 Then											;Key (dont save script keys)
					BufWriteString(stream,"")
				Else
					BufWriteString(stream,Tx\key$)
				EndIf
				BufWriteString(stream,Tx\value$)								;Value
				BufWriteString(stream,Tx\stuff$)								;Stuff
				If Tx\mode=6 Then Delete Tx
			EndIf
		Next		
	EndIf
	
	;Save Cam Angels (sav) + Player Position
	If mode$="sav" Then
		BufWriteFloat(stream,EntityPitch(cam))
		BufWriteFloat(stream,EntityYaw(cam))
	EndIf
	
	
	;### End
	bmpf_loadscreen(s$(20),100)
	con_add("")
	BufWriteLine(stream,"")
	BufWriteLine(stream,"### EOF Map File")
	BufWriteLine(stream,"www.unrealsoftware.de")
	
	;Attachments
	If mode$="map" Then
		For Tat.Tat=Each Tat
			If FileType(set_rootdir$+"mods\"+set_moddir$+"\"+Tat\path$)=1 Then
				atstream=BufReadFile(set_rootdir$+"mods\"+set_moddir$+"\"+Tat\path$)
				If atstream<>0 Then
					BufWriteLine(stream,strinv$(Tat\path$))
					BufWriteInt(stream,BufFileSize(set_rootdir$+"mods\"+set_moddir$+"\"+Tat\path$))
					While Not BufEof(atstream)
						byte=BufReadByte(atstream)
						byte=255-byte
						BufWriteByte(stream,byte)
					Wend
					BufCloseFile(atstream)
					atstream=0
				EndIf
			EndIf
		Next
	EndIf
	
	;Event: Postsave
	If m_section<>Csection_editor Then parse_globalevent("postsave")
	
	;Finish
	BufCloseFile(stream)
	Return 1
End Function


;### SAVE MAP IMAGE
Function save_map_image(mode$)
	Local pvimg=0
	Local ox#,oy#,oz#,o1#,o2#
	ox#=EntityX(cam)
	oy#=EntityY(cam)
	oz#=EntityZ(cam)
	o1#=EntityPitch(cam)
	o2#=EntityYaw(cam)
	;Create Image
	If map_image<>0 Then FreeImage(map_image)
	map_image=CreateImage(set_scrx,set_scry)
	;Get Previewimage Info
	If mode$="map" Then
		For Tinfo.Tinfo=Each Tinfo
			If Tinfo\typ=48 Then
				;Load Image
				If Tinfo\strings[0]<>"" Then
					pvimg=LoadImage(Tinfo\strings[0])
				EndIf
				;Set Cam Position and Rotation
				PositionEntity cam,EntityX(Tinfo\h),EntityY(Tinfo\h),EntityZ(Tinfo\h)
				RotateEntity cam,EntityPitch(Tinfo\h),EntityYaw(Tinfo\h),0
				;Ignore further Infos
				Exit
			EndIf
		Next
	EndIf
	;Render Image
	If m_section=Csection_editor Then
		If mb_sprite<>0 Then HideEntity mb_sprite
		For Tinfo.Tinfo=Each Tinfo
			HideEntity Tinfo\h
		Next
	EndIf
	RenderWorld()
	GrabImage map_image,0,0
	;Resize Image
	ResizeImage map_image,96,72
	;Add Previewimage
	If pvimg<>0 Then
		If ImageWidth(pvimg)<>96 Or ImageHeight(pvimg)<>72 Then
			ResizeImage pvimg,96,72
		EndIf
		SetBuffer ImageBuffer(map_image)
		DrawImage pvimg,0,0
		BufferDirty ImageBuffer(map_image)
		SetBuffer BackBuffer()
		FreeImage pvimg
	EndIf
	PositionEntity cam,ox#,oy#,oz#
	RotateEntity cam,o1#,o2#,0
End Function


;### SAVE MAP EXTENSIONS (PREPARE EXTENSIONS!)
Function save_map_extensions(mode$="",s_items=1)
	For Tx.Tx=Each Tx
		If Tx\mode=6 Then Delete Tx
	Next
	If mode$<>"map"
		;Drive/Ride
		Tx.Tx=New Tx
		Tx\mode=6
		Tx\key$="g_drive"
		Tx\value$=g_drive
		;Weather
		Tx.Tx=New Tx
		Tx\mode=6
		Tx\key$="env_cweather"
		Tx\value$=env_cweather
	EndIf
	;Timer 
	For Ttimer.Ttimer=Each Ttimer
		Tx.Tx=New Tx
		Tx\mode=6
		Tx\key$="t"
		Tx\parent_class=Ttimer\parent_class
		Tx\parent_id=Ttimer\parent_id
		start=gt-Ttimer\start
		If start<0 Then start=0
		Tx\value$=Ttimer\typ+","+Ttimer\duration+","+start+","+Ttimer\mode
		Tx\stuff$=Ttimer\scr$	
	Next
	;Current Weapon/Ammo
	If s_items=1 Then
		If con_unit(1) Then
			Tx.Tx=New Tx
			Tx\mode=6
			Tx\key$="w/a"
			Tx\value$=TCunit\player_weapon
			Tx\parent_class=TCunit\player_weapon
			Tx\parent_id=TCunit\player_ammo
		EndIf
	EndIf
	;FRAPs (free rotation and position)
	For Tfrap.Tfrap=Each Tfrap
		For c.Tfrap=Each Tfrap
			If Handle(Tfrap)<>Handle(c) Then
				If Tfrap\parent_class=c\parent_class Then
					If Tfrap\parent_id=c\parent_id Then
						Delete c
					EndIf
				EndIf
			EndIf
		Next
	Next
	For Tfrap.Tfrap=Each Tfrap
		Tx.Tx=New Tx
		Tx\mode=6
		Tx\key$="f"
		Tx\parent_class=Tfrap\parent_class
		Tx\parent_id=Tfrap\parent_id
		Tx\value$=Tfrap\y#+","+Tfrap\pitch#+","+Tfrap\roll#
	Next
	;Unit Path
	For Tup.Tup=Each Tup
		Tx.Tx=New Tx
		Tx\mode=6
		Tx\key$="u"
		Tx\parent_class=Tup\mode
		Tx\parent_id=Tup\info
		Tx\value$=Tup\unit
	Next
	;Freezes (i for ice)
	For Tunit.Tunit=Each Tunit
		If Tunit\freeze=1 Then
			Tx.Tx=New Tx
			Tx\mode=6
			Tx\key$="i"
			Tx\parent_class=Cclass_unit
			Tx\parent_id=Tunit\id
		EndIf
	Next
	;3D Texts
	For t3d.t3d=Each t3d
		Tx.Tx=New Tx
		Tx\mode=6
		Tx\key$="t3d"
		Tx\value$=t3d\txt$
		Tx\stuff$=t3d\typ+","+t3d\offset+","+t3d\range
		Tx\parent_class=t3d\class
		Tx\parent_id=T3d\id
	Next
	;Last Time
	Tx.Tx=New Tx
	Tx\mode=6
	Tx\key$="lasttime"
	Tx\stuff$=map_lday+","+map_lhour+","+map_lminute
End Function