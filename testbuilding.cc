
#include <iostream>
#include "ns3/netanim-module.h"

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/gnuplot.h"
#include "ns3/mobility-helper.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
#include "ns3/olsr-module.h"
#include <ns3/buildings-module.h>


using namespace ns3;
using namespace std;

// Main Code
int main (int argc, char *argv[])
{ 
	Ptr<Building> b = CreateObject <Building> ();
	b->SetBoundaries (Box (0.0, 100.0, 0.0, 100.0, 0.0, 10.0));
	b->SetBuildingType (Building::Residential);
	b->SetExtWallsType (Building::ConcreteWithWindows);
	b->SetNFloors (1);
	b->SetNRoomsX (2);
	b->SetNRoomsY (1); 
	
	float ap1_x = 40.0, ap1_y = 10.0, sta1_x = 65.0, sta1_y = 10.0;
	
	// Create AP Nodes
	NodeContainer wifiApNode;  
	wifiApNode.Create(1);
	
	// Create Client Nodes
	NodeContainer wifiStaNode;  
	wifiStaNode.Create(1);
	
	// Configure Nodes and  mobility Models
	MobilityHelper mobility;
	Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();

		
	//Initial position of AP and STA
	positionAlloc->Add (Vector (ap1_x, ap1_y, 0.5));
	positionAlloc->Add (Vector (sta1_x, sta1_y, 0.5)); 
	
	mobility.SetPositionAllocator (positionAlloc);
	mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
	mobility.Install (wifiApNodes.Get (0));
	mobility.Install (wifiStaNodes);
	
	BuildingsHelper bb;
	bb.Install (wifiApNodes);
	bb.Install (wifiStaNodes);
	
	//BuildingsHelper::Install (ueNodes);
	
	BuildingsHelper::MakeMobilityModelConsistent();
	
	
	 YansWifiChannelHelper channel;
	  channel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
	 	channel.AddPropagationLoss ("ns3::HybridBuildingsPropagationLossModel",
		"Frequency", DoubleValue (2.4 * 1e9),
		"Environment", StringValue("Urban"),
		"CitySize", StringValue("Small"),
		"ShadowSigmaOutdoor", DoubleValue (7.0),
		"ShadowSigmaIndoor", DoubleValue (8.0),
		"ShadowSigmaExtWalls", DoubleValue (5.0),
		"InternalWallLoss", DoubleValue (5.0));  

	  YansWifiPhyHelper phy =  YansWifiPhyHelper::Default ();
      phy.Set ("TxGain", DoubleValue (0));
	  phy.Set ("RxGain", DoubleValue (0));
	  phy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO);
	  phy.Set ("ShortGuardEnabled", BooleanValue (true));
	  phy.Set ("ChannelNumber", UintegerValue (36) );
	 
      phy.SetChannel (channel.Create ());

	  WifiHelper wifi;
	  wifi.SetStandard (WIFI_PHY_STANDARD_80211ac);
	  StringValue phyRate;
	 phyRate = StringValue ("VhtMcs0");

	  VhtWifiMacHelper mac = VhtWifiMacHelper::Default ();
  
	  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                               "DataMode",StringValue (phyRate),
                               "ControlMode",StringValue (phyRate));
	

	 NetDeviceContainer wifiapDevice[1], wifiStaDevice[1];
	 Ssid ssid1 = Ssid ("AP1");
	   
	 mac.SetType ("ns3::ApWifiMac",
				 "Ssid", SsidValue (ssid1),
				 "BeaconGeneration", BooleanValue (true),
				 "EnableBeaconJitter", BooleanValue (true),
				 "BeaconInterval",TimeValue (MicroSeconds (100000)),
				 "QosSupported", BooleanValue(true));
	    phy.Set ("TxPowerStart", DoubleValue (appower));
	    phy.Set ("TxPowerEnd", DoubleValue (appower));
		wifiapDevice[0] = wifi.Install(phy,mac,wifiApNode.Get(0));
	
	
}
