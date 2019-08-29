local prev_progress = 0

function progress(total, current)
    local ratio = current / total;
    ratio = math.min(math.max(ratio, 0), 1);
    local percent = math.floor(ratio * 100)
	if percent > prev_progress then
		print(" - NOMAD Node.js download progress (" .. percent .. "%/100%)")
	end
	prev_progress = percent
end

function verifyNodeInstallation()
    -- cmd needs absolute pathing
    local abs_path = path.getabsolute("")
	
	local download_dir = abs_path .. "/../../vendor/"
	
	local node_x86_dir = abs_path .. "/../../vendor/node-x86"
	local node_x64_dir = abs_path .. "/../../vendor/node-x64"

	local node_debug_test = "/Debug"
	local node_release_test = "/Release"
	
    -- ensure path existence
    if os.isdir(node_x86_dir .. node_debug_test) and os.isdir(node_x86_dir .. node_release_test) and os.isdir(node_x64_dir .. node_debug_test) and os.isdir(node_x64_dir .. node_release_test) then
        print(" - Node.js downloaded, skipping download")
        return;
    else
        os.mkdir(node_x86_dir .. "/Debug")
        os.mkdir(node_x86_dir .. "/Release")
        os.mkdir(node_x64_dir .. "/Debug")
        os.mkdir(node_x64_dir .. "/Release")

        print(" - installing NOMAD Node.js")
    end

    -- finally, do the download
    local target_url = "https://github.com/NomadGroup/node/releases/download/nomad-1.0/node-x64-debug.tar.gz"
    local result_str, response_code = http.download(target_url, (download_dir .. "/node-x64-debug.tar.gz"), {
        progress = progress,
        headers = { "From: Premake", "Referer: Premake" },
        userpwd = ""
    })
	
	target_url = "https://github.com/NomadGroup/node/releases/download/nomad-1.0/node-x64-release.tar.gz"
    result_str, response_code = http.download(target_url, (download_dir .. "/node-x64-release.tar.gz"), {
        progress = progress,
        headers = { "From: Premake", "Referer: Premake" },
        userpwd = ""
    })

	target_url = "https://github.com/NomadGroup/node/releases/download/nomad-1.0/node-x86-debug.tar.gz"
    result_str, response_code = http.download(target_url, (download_dir .. "/node-x86-debug.tar.gz"), {
        progress = progress,
        headers = { "From: Premake", "Referer: Premake" },
        userpwd = ""
    })
	
	target_url = "https://github.com/NomadGroup/node/releases/download/nomad-1.0/node-x86-release.tar.gz"
    result_str, response_code = http.download(target_url, (download_dir .. "/node-x86-release.tar.gz"), {
        progress = progress,
        headers = { "From: Premake", "Referer: Premake" },
        userpwd = ""
    })

    -- and unpack it
    if os.host() == "windows" then
        local sz_exe = abs_path .. "/../../../tools/premake/bin/win32/7z.exe"
		
        os.executef([[""%s" e "%s" "-o%s" "*.tar""]], sz_exe, download_dir .. "/node-x64-debug.tar.gz", download_dir)
        os.executef([[""%s" x "%s" "-o%s" "*.*" -r"]], sz_exe, download_dir .. "/node-x64-debug.tar", node_x64_dir .. "/Debug")
		
		
        os.executef([[""%s" e "%s" "-o%s" "*.tar""]], sz_exe, download_dir .. "/node-x64-release.tar.gz", download_dir)
        os.executef([[""%s" x "%s" "-o%s" "*.*" -r"]], sz_exe, download_dir .. "/node-x64-release.tar", node_x64_dir .. "/Release")
		
		
        os.executef([[""%s" e "%s" "-o%s" "*.tar""]], sz_exe, download_dir .. "/node-x86-debug.tar.gz", download_dir)
        os.executef([[""%s" x "%s" "-o%s" "*.*" -r"]], sz_exe, download_dir .. "/node-x86-debug.tar", node_x86_dir .. "/Debug")
		
		
        os.executef([[""%s" e "%s" "-o%s" "*.tar""]], sz_exe, download_dir .. "/node-x86-release.tar.gz", download_dir)
        os.executef([[""%s" x "%s" "-o%s" "*.*" -r"]], sz_exe, download_dir .. "/node-x86-release.tar", node_x86_dir .. "/Release")
    end

    return;
end
