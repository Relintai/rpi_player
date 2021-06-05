$(document).ready(function() {
    init_chart();

    $.getJSON("sensor_data").done(refresh_chart);
});

var myChart;

function init_chart() {
    var ctx = document.getElementById('myChart').getContext('2d');

    const data = {
        labels: [],
        datasets: [{
            label: 'Sensor #1',
            data: [],
            fill: false,
            borderColor: 'rgb(75, 192, 192)',
            tension: 0.1
        }]
    };

    const config = {
        type: 'line',
        data: data,
    };

    myChart = new Chart(ctx, config);
}

function refresh_chart(rec_data) {
    var labels = []
    var d = []

    var i;
    for (i = 0; i < rec_data.length; i++) {
        labels.push(rec_data[i].id);
        d.push(rec_data[i].measurement);
    }

    setData(myChart, labels, d);

    setTimeout(queue_update, 1000);
}

function setData(chart, labels, data) {

    chart.data.labels = labels;

    chart.data.datasets.forEach((dataset) => {
        dataset.data = data;
    });

    chart.update();
}

function queue_update() {
    $.getJSON("sensor_data").done(refresh_chart);
}