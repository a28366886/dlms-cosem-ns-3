/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Uniandes (unregistered)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Juanmalk <jm.aranda121@uniandes.edu.co> 
 */

#include "ns3/log.h"
#include "ns3/node.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/nstime.h"
#include "cosem-header.h"
#include "cosem-al-server.h"
#include "cosem-ap-server.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("CosemApplicationsProcessServer");
NS_OBJECT_ENSURE_REGISTERED (CosemApServer);

TypeId 
CosemApServer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::CosemApServer")
    .SetParent<Application> ()
    .AddConstructor<CosemApServer> ()
    ;
  return tid;
}

CosemApServer::CosemApServer ()
{
  NS_LOG_FUNCTION_NOARGS ();
  m_wPort = 0;
  m_udpPort = 0;
}

CosemApServer::~CosemApServer ()
{

}

void 
CosemApServer::Recv (int typeAcseService, int typeGet, int typeService, Ptr<Packet> packet)
{
  // COSEM-OPEN.ind
  if ((typeAcseService == OPEN) && (typeService == INDICATION))
    { 
      NS_LOG_INFO ("SAL-->OPEN.ind (S)");
      // Event: Invoke the COSEM-OPEN.res service
      Simulator::Schedule (Seconds (0.0), &CosemAlServer::CosemAcseOpen, m_cosemAlServer, RESPONSE, packet);
    }

  // COSEM-GET.ind
  if (typeService == INDICATION)
    {  
      if (typeGet == GET_NORMAL)
        { 
          NS_LOG_INFO ("SAL-->GET.ind (NORMAL) (S)");

          // Extract invoke-id-and-priority & cosem-attribute-descriptor parameters
          CosemGetRequestNormalHeader hdr;
          packet->RemoveHeader (hdr);
          m_reqData [0] = hdr.GetClassId (); 
          m_reqData [1] = hdr.GetInstanceId ();  
          m_reqData [2] = hdr.GetAttributeId ();

          // Future work: Invoke the Cosem interface model and return the data requested by the remote CAP
          uint32_t data = 0x251; // Ex. 593 kWh [long-unsigned, Page 22 of IEC 62056-62], 

          // Event: Invoke the COSEM-GET.res (NORMAL, Data) service
          Simulator::Schedule (Seconds (0.0), &CosemAlServer::CosemXdlmsGet, m_cosemAlServer, GET_NORMAL, RESPONSE, packet, data, hdr.GetInvokeIdAndPriority ()); 
        }
      else
        {
          NS_LOG_INFO ("Error: Undefined Get.ind Type (SAP)");     
        }    
    }
  else
    {
      NS_LOG_INFO ("Error: Undefined indicaction Cosem service (SAP)");     
    }
}

Ptr<Node>
CosemApServer::GetNode () const
{
  Ptr<Node> node = Application::GetNode ();
  return node;
}

void 
CosemApServer::SetCosemAlServer (Ptr<CosemAlServer> cosemAlServer)
{
  m_cosemAlServer = cosemAlServer;
}

Ptr<CosemAlServer> 
CosemApServer::GetCosemAlServer ()
{
  return m_cosemAlServer;
}

void
CosemApServer::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  Application::DoDispose ();
}

void 
CosemApServer::SetWport (uint16_t wPort)
{
  m_wPort = wPort;
}

uint16_t 
CosemApServer::GetWport ()
{
  return m_wPort;
}

void 
CosemApServer::SetUdpport (uint16_t udpPort)
{
  m_udpPort = udpPort ;
}

uint16_t 
CosemApServer::GetUdpport ()
{
  return m_udpPort;
}

void 
CosemApServer::SetLocalAddress (Address ip)
{
  m_localAddress = ip;
}

Address
CosemApServer::GetLocalAddress ()
{
  return m_localAddress;
}


void 
CosemApServer::StartApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();
}

void 
CosemApServer::StopApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();
}

} // namespace ns3


