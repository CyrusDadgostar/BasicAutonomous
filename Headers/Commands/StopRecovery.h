#pragma once
struct StopRecData
{
	CommandType commandType;
	bool isConfigured;
};

void initStopRecData()
{
	VERIFY(sizeof(StopData) < sizeof(BaseCommand));
	StopData data;
	data.commandType = StopRecovery;
	data.isConfigured = false;

	memcpy(&bot.commands[bot.index], &data, sizeof(StopData));

	bot.index++;
}

void configure(StopRecData* data)
{
	if(data->isConfigured) return;

	data->isConfigured = true;
}
