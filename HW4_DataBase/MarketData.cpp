#include "stdafx.h"

#include <blpapi_session.h>
#include <blpapi_eventdispatcher.h>

#include <blpapi_event.h>
#include <blpapi_message.h>
#include <blpapi_element.h>
#include <blpapi_name.h>
#include <blpapi_request.h>
#include <blpapi_subscriptionlist.h>
#include <blpapi_defs.h>
#include <blpapi_exception.h>

#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <utility>
#include <tchar.h>

using namespace std;
using namespace BloombergLP;
using namespace blpapi;

using BloombergLP::blpapi::Event;
using BloombergLP::blpapi::Element;
using BloombergLP::blpapi::Message;
using BloombergLP::blpapi::Name;
using BloombergLP::blpapi::Request;
using BloombergLP::blpapi::Service;
using BloombergLP::blpapi::Session;
using BloombergLP::blpapi::SessionOptions;


#import <C:\\Program Files\\Common Files\\System\\ado\\msado15.dll> rename( "EOF", "AdoNSEOF" )

_bstr_t bstrConnect = "Provider=Microsoft.Jet.OLEDB.4.0; Data Source=C:\\Users\\FREUser\\Desktop\\MarketDataDB.mdb;";

namespace
{
	const Name SECURITY_DATA("securityData");
	const Name SECURITY_NAME("security");
	const Name BLPDATE("date");

	const Name FIELD_ID("fieldId");
	const Name FIELD_DATA("fieldData");
	const Name FIELD_DESC("description");
	const Name FIELD_INFO("fieldInfo");
	const Name FIELD_ERROR("fieldError");
	const Name FIELD_MSG("message");

	const Name BEST_SALES("BEST_SALES");

	const Name SECURITY_ERROR("securityError");
	const Name ERROR_MESSAGE("message");
	const Name FIELD_EXCEPTIONS("fieldExceptions");
	const Name ERROR_INFO("errorInfo");
}

class TradeData
{
private:
	string sDate;
	double dOpen;
	double dClose;
	double dHigh;
	double dLow;
	long lVolume;
public:
	TradeData() : sDate(""), dOpen(0), dClose(0), dHigh(0), dLow(0), lVolume(0) {}
	TradeData(string sDate_, double dOpen_, double dClose_, double dHigh_, double dLow_, long lVolume_) :
		sDate(sDate_), dOpen(dOpen_), dClose(dClose_), dHigh(dHigh_), dLow(dLow_), lVolume(lVolume_) {}
	TradeData(const TradeData & TradeData) :sDate(TradeData.sDate), dOpen(TradeData.dOpen),
		dClose(TradeData.dClose), dHigh(TradeData.dHigh), dLow(TradeData.dLow), lVolume(TradeData.lVolume) {}
	TradeData operator=(const TradeData & TradeData)
	{
		sDate = TradeData.sDate;
		dOpen = TradeData.dOpen;
		dClose = TradeData.dClose;
		dHigh = TradeData.dHigh;
		dLow = TradeData.dLow;
		lVolume = TradeData.lVolume;

		return *this;
	}
	string getDate() { return sDate; }
	double getOpen() { return dOpen;  }
	double getClose() { return dClose; }
	double getHigh() { return dHigh;  }
	double getLow() { return dLow; }
	long getVolume() { return lVolume; }
	friend ostream & operator<<(ostream & ostr, const TradeData & TradeData)
	{
		ostr << TradeData.sDate << " " << TradeData.dOpen << " " << TradeData.dClose << " " << TradeData.dHigh << " " << TradeData.dLow << " " << TradeData.lVolume << endl;
		return ostr;
	}
};

class Stock
{
private:
	string sSymbol;
	vector<TradeData> trades;

public:
	Stock() : sSymbol("") {}
	Stock(string sSymbol_, const vector<TradeData> trades_) :sSymbol(sSymbol_), trades(trades_) {}
	Stock(const Stock & stock) :sSymbol(stock.sSymbol), trades(stock.trades) {}
	Stock operator=(const Stock & stock)
	{
		sSymbol = stock.sSymbol;
		trades = stock.trades;

		return *this;
	}

	void addTrade(const TradeData & trade) { trades.push_back(trade); }
	string getSymbol() { return sSymbol;  }
	const vector<TradeData> & getTrades() { return trades; }

	friend ostream & operator<<(ostream & ostr, const Stock & stock)
	{
		ostr << "Symbol: " << stock.sSymbol << endl;
		for (vector<TradeData>::const_iterator itr = stock.trades.begin(); itr != stock.trades.end(); itr++)
			ostr << *itr;
		return ostr;
	}
};

class MarketData
{
private:
	std::string         sHost;
	int                 iPort;
	vector<Stock>		stockMap;

	void printUsage()
	{
		std::cout << "Usage:" << std::endl
			<< "    Retrieve reference data " << std::endl
			<< "        [-ip        <ipAddress  = localhost>" << std::endl
			<< "        [-p         <tcpPort    = 8194>" << std::endl;
	}

	bool parseCommandLine(int argc, char **argv)
	{
		for (int i = 1; i < argc; ++i) {
			if (!std::strcmp(argv[i], "-ip") && i + 1 < argc) {
				sHost = argv[++i];
				continue;
			}
			if (!std::strcmp(argv[i], "-p") && i + 1 < argc) {
				iPort = std::atoi(argv[++i]);
				continue;
			}
			printUsage();
			return false;
		}
		return true;
	}

	void PrintComError(_com_error &e)
	{
		_bstr_t bstrSource(e.Source());
		_bstr_t bstrDescription(e.Description());

		// Print Com errors.
		printf("Error\n");
		printf("\tCode = %08lx\n", e.Error());
		printf("\tCode meaning = %s\n", (char *)e.ErrorMessage());
		printf("\tSource = %s\n", (LPCSTR)bstrSource);
		printf("\tDescription = %s\n", (LPCSTR)bstrDescription);
	}

public:
	void PrintStockMap()
	{
		ofstream fout("stockMap.txt");
		vector<Stock>::iterator itr = stockMap.begin();
		for (; itr != stockMap.end(); itr++)
			fout << *itr;
		fout.close();
	}
	void run(int argc, char **argv)
	{
		ofstream fout;
		fout.open("output.txt");

		sHost = "localhost";
		iPort = 8194;
		if (!parseCommandLine(argc, argv)) return;

		SessionOptions sessionOptions;
		sessionOptions.setServerHost(sHost.c_str());
		sessionOptions.setServerPort(iPort);

		std::cout << "Connecting to " << sHost << ":" << iPort << std::endl;
		Session session(sessionOptions);
		if (!session.start()) {
			std::cerr << "Failed to start session." << std::endl;
			return;
		}
		if (!session.openService("//blp/refdata")) {
			std::cerr << "Failed to open //blp/refdata" << std::endl;
			return;
		}
		Service refDataService = session.getService("//blp/refdata");

		Request request = refDataService.createRequest("HistoricalDataRequest");

		request.getElement("fields").appendValue("OPEN");
		request.getElement("fields").appendValue("LAST_TRADE");
		request.getElement("fields").appendValue("VOLUME");
		request.getElement("fields").appendValue("HIGH");
		request.getElement("fields").appendValue("LOW");
		request.set("periodicityAdjustment", "ACTUAL");
		request.set("periodicitySelection", "DAILY");
		request.set("startDate", "20180101");
		request.set("endDate", "20181231");
		request.set("maxDataPoints", 2000);

		vector<string> stockList;
		stockList.push_back("GOOG");
		stockList.push_back("IBM");
		stockList.push_back("MSFT");
		stockList.push_back("TWTR");
		stockList.push_back("");
		string sSuffix = " US Equity";
		vector<string>::iterator vitr = stockList.begin();
		for (; vitr != stockList.end(); vitr++)
		{
			string sStock = *vitr + sSuffix;
			request.getElement("securities").appendValue(sStock.c_str());
		}
		std::cout << "Sending Request: " << request << std::endl;
		session.sendRequest(request);

		while (true)
		{
			Event event = session.nextEvent();
			MessageIterator msgIter(event);
			string sSymbol = "";
			vector<TradeData> trades;
			while (msgIter.next()) {
				Message msg = msgIter.message();
				if ((event.eventType() != Event::PARTIAL_RESPONSE) && (event.eventType() != Event::RESPONSE))
					continue;

				msg.asElement().print(fout);
				fout << endl;

				Element securityData = msg.getElement(SECURITY_DATA);
				sSymbol = securityData.getElement("security").getValueAsString();
				std::size_t found = sSymbol.find(" ");
				if (found != std::string::npos)
					sSymbol = sSymbol.substr(0, found);
				Element fieldData = securityData.getElement(FIELD_DATA);
				for (int i = 0; i < int(fieldData.numValues()); i++)
				{
					string sDate = fieldData.getValueAsElement(i).getElement("date").getValueAsString();
					double fClose = fieldData.getValueAsElement(i).getElement("LAST_TRADE").getValueAsFloat64();
					double fOpen = fieldData.getValueAsElement(i).getElement("OPEN").getValueAsFloat64();
					double fHigh = fieldData.getValueAsElement(i).getElement("HIGH").getValueAsFloat64();
					double fLow = fieldData.getValueAsElement(i).getElement("LOW").getValueAsFloat64();
					long lVolume = (long)fieldData.getValueAsElement(i).getElement("VOLUME").getValueAsInt64();
					TradeData aTrade(sDate, fOpen, fClose, fHigh, fLow, lVolume);
					trades.push_back(aTrade);
				}
			}
			if (sSymbol.length() > 0)
				stockMap.push_back(Stock(sSymbol, trades));

			if (event.eventType() == Event::RESPONSE)
				break;
		}
		for (vector<Stock>::iterator itr = stockMap.begin(); itr != stockMap.end(); itr++)
			cout << *itr;
	}

	int UpdateTradeDBTable()
	{

		HRESULT hResult = CoInitialize(0);
		if (FAILED(hResult))
		{
			return -1;
		}
		try
		{
			ADODB::_ConnectionPtr pConnect("ADODB.Connection");
			hResult = pConnect->Open(bstrConnect, "admin", "", ADODB::adConnectUnspecified);
			if (SUCCEEDED(hResult))
			{
				char sQuery[256];
				memset((void*)sQuery, '\0', 256);

				sprintf_s(sQuery, "DELETE * FROM DailyData;");
				ADODB::_RecordsetPtr pRecSet("ADODB.Recordset");
				hResult = pRecSet->Open(sQuery, _variant_t((IDispatch *)pConnect, true), ADODB::adOpenUnspecified,
					ADODB::adLockUnspecified, ADODB::adCmdText);

				memset((void*)sQuery, '\0', 256);
				for (vector<Stock>::iterator mIt = stockMap.begin(); mIt != stockMap.end(); mIt++)
				{
					string sSymbol = mIt->getSymbol();
					vector<TradeData> trades = mIt->getTrades();
					for (vector<TradeData>::iterator vIt = trades.begin(); vIt != trades.end(); vIt++)
					{
						string sDate = vIt->getDate();
						double dOpen = vIt->getOpen();
						double dClose = vIt->getClose();
						double dHigh = vIt->getHigh();
						double dLow = vIt->getLow();
						long lVolume = vIt->getVolume();
						sprintf_s(sQuery, "INSERT INTO DailyData VALUES('%s','%s',%.2f,%.2f,%.2f,%.2f,%ld);", sDate.c_str(), sSymbol.c_str(), dOpen, dClose, dHigh, dLow, lVolume);
						ADODB::_RecordsetPtr pRecSet("ADODB.Recordset");
						hResult = pRecSet->Open(sQuery, _variant_t((IDispatch *)pConnect, true), ADODB::adOpenUnspecified,
							ADODB::adLockUnspecified, ADODB::adCmdText);
					}
				}
				pConnect->Close();
			}
		}
		catch (_com_error& e)
		{
			// Handle Exception
			PrintComError(e);
		}

		// Release COM
		CoUninitialize();

		return 0;
	}
};

int main(int argc, char **argv)
{
	std::cout << "Market Data" << std::endl;
	MarketData marketData;
	marketData.run(argc, argv);
	marketData.PrintStockMap();
	marketData.UpdateTradeDBTable();
	return 0;
}

