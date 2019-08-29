
-- Copyright (c) Nomad Group 2019

function progress(total, current)
    local ratio = current / total;
    ratio = math.min(math.max(ratio, 0), 1);
    local percent = math.floor(ratio * 100);
	
	if percent == 100 then
		print("successfully downloaded")
	end
	
	--print(" - CEF download progress (" .. percent .. "%/100%)")
end

function verifycef(version, platform)

	local avpath = "../vendor/cef/" .. platform
	
	-- fix encoding issue
	CEFDIR = string.gsub(version, "(%%2B)", "+") .. platform
	
	-- does it exist?
	if os.isdir(avpath) or os.isdir(avpath .. "/cur_dl") then
		print("> cef exists, skipping installation")
		
		if _ACTION == "clean" then
			os.rmdir(avpath)
			os.rmdir(avpath .. "/cur_dl")
		end
		
		return;
	else
		os.mkdir(avpath)
		os.mkdir(avpath .. "/cur_dl")
		
		print("installing cef " .. version)
	end
	
	-- next, download the arc
	local target_url = "http://opensource.spotify.com/cefbuilds/" .. version .. platform .. ".tar.bz2"
	print("fetching cef from " .. target_url)
	
    local result_str, response_code = http.download(target_url, avpath .. "/cur_dl/cef.tar.bz2", {
        progress = progress,
        headers = { "From: Premake", "Referer: Premake" },
        userpwd = ""
    })
	
	local abs_path = path.getabsolute('')
	
	
	-- now unpack it
	if os.host() == "windows" then
		local sevenzip = abs_path .. "/../../tools/premake/bin/win32/7z.exe"
		
		-- extract bz2
		local src = abs_path .. "/" .. avpath .. "/cur_dl/cef.tar.bz2"
		local dest = abs_path .. "/" .. avpath .. "/cur_dl"

		os.executef("%s e %s -o%s *.tar", sevenzip, src, dest)
		
		-- extract tarball
		src = abs_path .. "/" .. avpath .. "/cur_dl/cef.tar"
        dest = abs_path .. "/" .. avpath

		os.executef("%s x %s -o%s %s\\*.* -r", sevenzip, src, dest, CEFDIR)
	end
	
	-- finally, copy the resources
	os.rmdir(avpath .. "/cur_dl")

	local tarch = "Win32"
	if platform == "windows64" then
		tarch = "Win64"
	end
	
	local croot = avpath .. "/" .. CEFDIR
	print("ROOT IS " .. croot)
	
	-- move that shit
	os.execute("{COPY} " .. croot .. "/Debug ../../bin/" .. tarch .. "/Debug")
	os.execute("{COPY} " .. croot .. "/Resources ../../bin/" .. tarch .. "/Debug")

	os.execute("{COPY} " .. croot .. "/Release ../../bin/" .. tarch .. "/Release")
	os.execute("{COPY} " .. croot .. "/Resources ../../bin/" .. tarch .. "/Release")

	os.execute("{COPY} " .. croot .. "/Release ../../bin/" .. tarch .. "/Shipping")
	os.execute("{COPY} " .. croot .. "/Resources ../../bin/" .. tarch .. "/Shipping")

    return;
end
