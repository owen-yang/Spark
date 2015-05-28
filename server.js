var spark = require('spark')
spark.login({accessToken: 'f9cf70a78e732b1c1067c2d8c10de165d2443739'})


spark.getDevice('domcore0', function(err, device) {
    console.log(device.name + ' connected');
    console.log('time: ' + Date.now())

    device.subscribe('dust_sense', function(data) {
        data = JSON.parse(data.data)
        console.log(data)
    })

    setInterval(function () {
        device.callFunction('dust', '', function(err, data) {
            if (err) console.log('ERROR:', err)
        })
    },15000)
    
    device.callFunction('dust', '', function(err, data) {
        if (err) console.log('ERROR:', err)
    })
})
