function mathTest()
	local pow = math.random(6, 12)
	local coeff = math.random(13,44)
	local digit = math.random(-9999,9999)
	
	local result = math.pow(2, pow) + 11.9 * coeff + digit
	
	local str = string.format("2^%i + 11.9*%i + %i = %.1f", pow, coeff, digit, result)
	
	return "<p>" .. str .. "</p>"
end

function dateTime()
	return "<p>The current date and time is: " .. os.date("%x %X") .. "</p>"
end

return [[
<html>
<head>
	<title>Lua test</title>
</head>
<body>
	<h1>Lua test</h1>
	]] .. mathTest() .. dateTime() .. [[
</body>
</html>]]