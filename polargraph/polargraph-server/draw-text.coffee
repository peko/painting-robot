# PORT = "/dev/ttyUSB0"

GOST = require("./GOST").GOST

#PORT = "COM9"
PORT = "/dev/ttyUSB0"

FILE ="./data/test.d"
SCALE = 5.0

SerialPort = require("serialport").SerialPort
serialPort = require "serialport"

data = []
count = 0

readFile = (callback) ->
    fs = require  "fs"
    fs.readFile FILE, "utf-8", (err, d)->
        if err then return console.log err 
        callback d

listPorts = ->
    serialPort.list (err, ports)->
        ports.forEach (port)->
            console.log port.comName
            console.log port.pnpId
            console.log port.manufacturer


scale = (s)->
    p = s.split(",")
    if p.length == 2 and parseFloat(p[0]) != NaN and parseFloat(p[1]) != NaN
        return "#{(parseFloat(p[0])*SCALE).toFixed(4)},#{(parseFloat(p[1])*SCALE).toFixed(4)}"
    s

pushNext = ()->

    c = data[count++];

    if !c
        console.log "<< [M 0.0,0.0]"
        serialPort.write "М 0.0,0.0 "
        setTimeout (=>process.exit 0), 100
    
    switch c
        when 'm'
            command = "m #{scale(data[count++])}"
        when 'M'
            command = "M #{scale(data[count++])}"
        when 'l' 
            command =  "l #{scale(data[count++])}"
        when 'L' 
            command =  "L #{scale(data[count++])}"
        when 'z'
            command =  "z"
        when 'Z'
            command =  "z"
        else
            p = c.split(",")
            if p.length == 2 and parseFloat(p[0]) != NaN and parseFloat(p[1]) != NaN
                command = "l #{scale(c)}"
            else
                console.log "?[#{c}]" 
                return


    console.log "<< [#{command}]"
    serialPort.write "#{command} "
    

    if count >= data.length then process.exit 0

onData =  (d)->
    console.log ">>[#{d}]"
    switch d
        when "RDY"
            pushNext()
        when "OK"
            pushNext()


openSerial = (data, callback)->

    serialPort = new SerialPort PORT, 
        baudrate: 115200
        parser: serialPort.parsers.readline("\r\n")

    serialPort.on "open",  ()->
        console.log "open"
        serialPort.on "data", onData


console.log data


text = "АНТОН! УХОДИ!"

lh   = 15.0*SCALE
line = 0
start_x = -200.0/SCALE
start_y = -250.0/SCALE

code = "M #{start_x},#{start_y} ";

for c in text
    if c is " "
        code += "m 6.0,0.0 "
    if c is "\n"
        code += "M #{start_x},#{start_y+(++line)*lh} "
    else
        d = GOST[c]
        if not d then continue
        code += d.d + "m #{d.w+2.0},0.0 "

data = code.split " "   
openSerial()
