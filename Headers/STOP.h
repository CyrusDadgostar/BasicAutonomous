#pragma once
struct StopData
{
	CommandType commandType;
	bool isConfigured;
};

void initStopData()
{
	VERIFY(sizeof(StopData) < sizeof(BaseCommand));
	StopData data;
	data.commandType = Stop;
	data.isConfigured = false;

	memcpy(&bot.commands[bot.index], &data, sizeof(StopData));

	bot.index++;
}

void configure(StopData* data)
{
	if(data->isConfigured) return;

	data->isConfigured = true;
}
