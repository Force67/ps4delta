typename = { "client","server", "everything" }

newoption {
	trigger		= "type",
	value 		= "TARGET",
	description = "Choose a game to target",
	allowed 	= {
		{ "client",   "Watch Dogs 2 Multiplayer Client" },
		{ "server", "Watch Dogs 2 Multiplayer Server" },
		{ "everything", "Build everything" }
	}
}

newoption {
	trigger		= "is_ci_build",
	description	= "Toggle CI Build Options.",
	allowed 	= {
		{ "false",   "No CI Build defs" },
		{ "true", "Enabled CI Build defs" }
	}
}

if not _OPTIONS['type'] then
	_OPTIONS['type'] = 'unknown'
end

if not _OPTIONS['is_ci_build'] then
	_OPTIONS['is_ci_build'] = 'false'
end