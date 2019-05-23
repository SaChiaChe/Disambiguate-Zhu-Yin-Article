LOADMAPFILE = "Big5-ZhuYin.map"
SAVEMAPFILE = "ZhuYin-Big5.map"

if __name__ == "__main__":
	LenZhuYin = 37
	ZhuYinList = "ㄅㄆㄇㄈㄉㄊㄋㄌㄍㄎㄏㄐㄑㄒㄓㄔㄕㄖㄗㄘㄙㄧㄨㄩㄚㄛㄜㄝㄞㄟㄠㄡㄢㄣㄤㄥㄦ"
	
	# Read in Map and construct dictionary(using list lol)
	TrackZhuYin = [[x] for x  in ZhuYinList]
	with open(LOADMAPFILE, "r", encoding = "cp950") as f:
		for line in f:
			Line = line.strip("\n").split(" ", 1)
			Char = Line[0]
			ZhuYin = Line[1].split("/")
			temp = set()
			for ZY in ZhuYin:
				temp.add(ZY[0])
			for i in temp:
				TrackZhuYin[ZhuYinList.index(i)].append(Char)

	Redefine = []
	# Save our mapping to file
	with open(SAVEMAPFILE, "w+", encoding = "cp950") as f:
		for Z in TrackZhuYin:
			z, Chars = Z[0], Z[1:]
			# If no characters start with z, then don't write
			if len(Chars) == 0:
				continue
				
			# Write first line
			f.write(z + " ")
			for c in Chars:
				f.write(c + " ")

			# Write the characters on each line
			f.write("\n")
			for c in Chars:
				if c not in Redefine:
					f.write(c + " " + c + "\n")
					Redefine.append(c)