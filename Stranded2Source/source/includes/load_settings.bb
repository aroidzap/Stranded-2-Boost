;############################################ SETTINGS

;### Load Settings
Function load_settings()
	;Controls
	Dim in_keys(20)
	Local stream=BufReadFile("sys\controls.cfg")
	If stream=0 Then
		;Failed? Set Defaults!
		set_msens#=0.2
		set_minvert=1
		set_msmooth=0
	Else
		;Load
		BufReadLine(stream)
		For i=0 To 20
			in_keys(i)=BufReadShort(stream)
		Next
		set_msens#=BufReadFloat(stream)
		set_minvert=BufReadByte(stream)-128
		set_msmooth=BufReadByte(stream)
		BufCloseFile(stream)
	EndIf
	;Scriptcontrols
	Dim game_scriptkey(20)
	stream=BufReadFile("sys\scriptcontrols.cfg")
	If stream<>0 Then
		BufReadLine(stream)
		For i=0 To 20
			game_scriptkey(i)=BufReadShort(stream)
		Next
		BufCloseFile(stream)
	EndIf
	;Settings
	stream=BufReadFile("sys\settings.cfg")
	If stream=0 Then
		;Failed? Set Defaults!
		set_scrx=800
		set_scry=600
		set_scrbit=32
		set_hwmultitex=1
		set_mb=1
		set_mb_alpha#=0.35
	Else
		;Load
		BufReadLine(stream)
		set_scrx=BufReadLine(stream):set_scrx_c=set_scrx
		set_scry=BufReadLine(stream):set_scry_c=set_scry
		set_scrbit=BufReadLine(stream):set_scrbit_c=set_scrbit
		set_viewrange=BufReadLine(stream)
		set_terrain=BufReadLine(stream)
		set_water=BufReadLine(stream)
		set_sky=BufReadLine(stream)
		set_effects=BufReadLine(stream)
		set_musicvolume#=BufReadLine(stream)
		set_fxvolume#=BufReadLine(stream)
		set_grass=BufReadLine(stream)
		set_2dfx=BufReadLine(stream)
		set_lightfx=BufReadLine(stream)
		set_windsway=BufReadLine(stream)
		set_name$=BufReadLine(stream)
		set_serverport=BufReadLine(stream)
		set_fog=BufReadLine(stream)
		set_hwmultitex=BufReadLine(stream)
		set_mb=BufReadLine(stream)
		set_mb_alpha#=BufReadLine(stream)
		BufCloseFile(stream)
	EndIf
	
	;Setup
	
	;Viewrange
	Select set_viewrange
		Case 0 set_viewfac#=0.5
		Case 1 set_viewfac#=1.0
		Case 2 set_viewfac#=1.5
		Case 3 set_viewfac#=2.5
		Case 4 set_viewfac#=4.0
		Default set_viewfac#=1.5
	End Select
	
	;Motion Blur
	If mb_sprite<>0 Then
		EntityAlpha mb_sprite,set_mb_alpha#
		If set_mb_alpha#>Cpeakblur# Then EntityAlpha mb_sprite,Cpeakblur#
	EndIf
	
End Function

load_settings()


;### Save Settings
Function save_settings()
	;Controls
	Local stream=BufWriteFile("sys\controls.cfg")
	If stream=0 Then RuntimeError("Unable to write sys\controls.cfg")
	BufWriteLine(stream,"### "+set_moddir$+" Control Settings (Warning: Editing this File may cause Errors)")
	For i=0 To 20
		BufWriteShort(stream,in_keys(i))
	Next
	BufWriteFloat(stream,set_msens#)
	BufWriteByte(stream,set_minvert+128)
	BufWriteByte(stream,set_msmooth)
	BufCloseFile(stream)
	;Scriptcontrols
	If game_scriptkeys>0 Then
		stream=BufWriteFile("sys\scriptcontrols.cfg")
		If stream=0 Then RuntimeError("Unable to write sys\scriptcontrols.cfg")
		BufWriteLine(stream,"### "+set_moddir$+" Script Control Settings (Warning: Editing this File may cause Errors)")
		For i=0 To 20
			BufWriteShort(stream,game_scriptkey(i))
		Next
		BufCloseFile(stream)
	EndIf
	;Settings
	stream=BufWriteFile("sys\settings.cfg")
	If stream=0 Then RuntimeError("Unable to write sys\settings.cfg")
	BufWriteLine(stream,"### "+set_moddir$+" Settings (Warning: Editing this File may cause Errors)")
	BufWriteLine(stream,set_scrx_c)
	BufWriteLine(stream,set_scry_c)
	BufWriteLine(stream,set_scrbit_c)
	BufWriteLine(stream,set_viewrange)
	BufWriteLine(stream,set_terrain)
	BufWriteLine(stream,set_water)
	BufWriteLine(stream,set_sky)
	BufWriteLine(stream,set_effects)
	BufWriteLine(stream,set_musicvolume#)
	BufWriteLine(stream,set_fxvolume#)
	BufWriteLine(stream,set_grass)
	BufWriteLine(stream,set_2dfx)
	BufWriteLine(stream,set_lightfx)
	BufWriteLine(stream,set_windsway)
	set_name$=mp_checkname(set_name$,0)
	BufWriteLine(stream,set_name$)
	BufWriteLine(stream,set_serverport)
	BufWriteLine(stream,set_fog)
	BufWriteLine(stream,set_hwmultitex)
	BufWriteLine(stream,set_mb)
	BufWriteLine(stream,set_mb_alpha#)
	BufCloseFile(stream)
	
	;Setup
	
	;Viewrange
	Select set_viewrange
		Case 0 set_viewfac#=0.5
		Case 1 set_viewfac#=1.
		Case 2 set_viewfac#=1.5
		Case 3 set_viewfac#=2.
		Case 4 set_viewfac#=2.5
		Default set_viewfac#=1.5
	End Select
	
	;Motion Blur
	If mb_sprite<>0 Then
		EntityAlpha mb_sprite,set_mb_alpha#
		If set_mb_alpha#>Cpeakblur# Then EntityAlpha mb_sprite,Cpeakblur#
	EndIf
	
	;Water
	e_environment_setup_water()
	
	;Grass
	grass_map()
	grass_heightmap()
	grass_x=-2147483648
	grass_y=-2147483648
	grass_spread()
End Function