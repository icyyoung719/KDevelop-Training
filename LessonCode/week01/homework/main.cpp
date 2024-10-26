﻿#include<iostream>
#include "Scanner.h"
#include "Parser.h"

int main() {
    using namespace json;
    auto source = R"(
        {
            "name": "John",
            "age": 30,
            "age2": true,
            "player":{
                "name":"Tom",
                "age": 20
            },
            "scores":[
                1,
                2,
                3,
            ]
        }
    )";

    auto source2 = R"(
{
    "time": "2018-09-22 12:37:21",
    "cityInfo": {
        "city": "天津市",
        "cityId": "101030100",
        "parent": "天津",
        "updateTime": "12:32"
    },
    "date": "20180922",
    "message": "Success !",
    "status": 200,
    "data": {
        "shidu": "22%",
        "pm25": 15.0,
        "pm10": 46.0,
        "quality": "优",
        "wendu": "24",
        "ganmao": "各类人群可自由活动",
        "yesterday": {
            "date": "21",
            "ymd": "2018-09-21",
            "week": "星期五",
            "sunrise": "05:56",
            "high": "高温 25.0℃",
            "low": "低温 15.0℃",
            "sunset": "18:12",
            "aqi": 108.0,
            "fx": "西北风",
            "fl": "4-5级",
            "type": "晴",
            "notice": "愿你拥有比阳光明媚的心情"
        },
        "forecast": [
            {
                "date": "22",
                "ymd": "2018-09-22",
                "week": "星期六",
                "sunrise": "05:57",
                "high": "高温 26.0℃",
                "low": "低温 15.0℃",
                "sunset": "18:10",
                "aqi": 55.0,
                "fx": "西北风",
                "fl": "4-5级",
                "type": "晴",
                "notice": "愿你拥有比阳光明媚的心情"
            },
            {
                "date": "23",
                "ymd": "2018-09-22",
                "week": "星期日",
                "sunrise": "05:58",
                "high": "高温 23.0℃",
                "low": "低温 14.0℃",
                "sunset": "18:09",
                "aqi": 29.0,
                "fx": "西北风",
                "fl": "4-5级",
                "type": "晴",
                "notice": "愿你拥有比阳光明媚的心情"
            },
            {
                "date": "24",
                "ymd": "2018-09-22",
                "week": "星期一",
                "sunrise": "05:59",
                "high": "高温 24.0℃",
                "low": "低温 15.0℃",
                "sunset": "18:07",
                "aqi": 25.0,
                "fx": "西北风",
                "fl": "<3级",
                "type": "晴",
                "notice": "愿你拥有比阳光明媚的心情"
            },
            {
                "date": "25",
                "ymd": "2018-09-22",
                "week": "星期二",
                "sunrise": "06:00",
                "high": "高温 24.0℃",
                "low": "低温 16.0℃",
                "sunset": "18:05",
                "aqi": 56.0,
                "fx": "西南风",
                "fl": "<3级",
                "type": "晴",
                "notice": "愿你拥有比阳光明媚的心情"
            },
            {
                "date": "26",
                "ymd": "2018-09-22",
                "week": "星期三",
                "sunrise": "06:01",
                "high": "高温 24.0℃",
                "low": "低温 17.0℃",
                "sunset": "18:04",
                "aqi": 86.0,
                "fx": "西南风",
                "fl": "3-4级",
                "type": "阴",
                "notice": "不要被阴云遮挡住好心情"
            }
        ]
    }
}
    )";

    Scanner scanner(source2);
    Parser parser(scanner);

    JsonElement* element = parser.Parse();
    std::cout << element->toString() << std::endl;

    delete element;
    return 0;
}