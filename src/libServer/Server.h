/*
 * Copyright (c) 2018 Zilliqa
 * This source code is being disclosed to you solely for the purpose of your
 * participation in testing Zilliqa. You may view, compile and run the code for
 * that purpose and pursuant to the protocols and algorithms that are programmed
 * into, and intended by, the code. You may not do anything else with the code
 * without express permission from Zilliqa Research Pte. Ltd., including
 * modifying or publishing the code (or any part of it), and developing or
 * forming another public or private blockchain network. This source code is
 * provided 'as is' and no warranties are given as to title or non-infringement,
 * merchantability or fitness for purpose and, to the extent permitted by law,
 * all liability for your use of the code is disclaimed. Some programs in this
 * code are governed by the GNU General Public License v3.0 (available at
 * https://www.gnu.org/licenses/gpl-3.0.en.html) ('GPLv3'). The programs that
 * are governed by GPLv3.0 are those programs that are located in the folders
 * src/depends and tests/depends and which include a reference to GPLv3 in their
 * program files.
 */

#include <jsonrpccpp/server.h>
#include <jsonrpccpp/server/connectors/httpserver.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <boost/multiprecision/cpp_int.hpp>
#pragma GCC diagnostic pop
#include <mutex>
#include "libData/BlockData/BlockHeader/BlockHeaderBase.h"
#include "libData/DataStructures/CircularArray.h"

class Mediator;

class AbstractZServer : public jsonrpc::AbstractServer<AbstractZServer> {
 public:
  AbstractZServer(jsonrpc::AbstractServerConnector& conn,
                  jsonrpc::serverVersion_t type = jsonrpc::JSONRPC_SERVER_V2)
      : jsonrpc::AbstractServer<AbstractZServer>(conn, type) {
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetNetworkId", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_STRING, NULL),
        &AbstractZServer::GetNetworkIdI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("CreateTransaction", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_OBJECT, "param01",
                           jsonrpc::JSON_OBJECT, NULL),
        &AbstractZServer::CreateTransactionI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetTransaction", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_OBJECT, "param01",
                           jsonrpc::JSON_STRING, NULL),
        &AbstractZServer::GetTransactionI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetDsBlock", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_OBJECT, "param01",
                           jsonrpc::JSON_STRING, NULL),
        &AbstractZServer::GetDsBlockI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetTxBlock", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_OBJECT, "param01",
                           jsonrpc::JSON_STRING, NULL),
        &AbstractZServer::GetTxBlockI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetLatestDsBlock", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_OBJECT, NULL),
        &AbstractZServer::GetLatestDsBlockI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetLatestTxBlock", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_OBJECT, NULL),
        &AbstractZServer::GetLatestTxBlockI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetBalance", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_OBJECT, "param01",
                           jsonrpc::JSON_STRING, NULL),
        &AbstractZServer::GetBalanceI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetMinimumGasPrice", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_STRING, NULL),
        &AbstractZServer::GetMinimumGasPriceI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetPrevDSDifficulty", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_INTEGER, NULL),
        &AbstractZServer::GetPrevDSDifficultyI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetPrevDifficulty", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_INTEGER, NULL),
        &AbstractZServer::GetPrevDifficultyI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetSmartContracts", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_ARRAY, "param01", jsonrpc::JSON_STRING,
                           NULL),
        &AbstractZServer::GetSmartContractsI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetContractAddressFromTransactionID",
                           jsonrpc::PARAMS_BY_POSITION, jsonrpc::JSON_STRING,
                           "param01", jsonrpc::JSON_STRING, NULL),
        &AbstractZServer::GetContractAddressFromTransactionIDI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("CreateMessage", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_STRING, "param01",
                           jsonrpc::JSON_OBJECT, NULL),
        &AbstractZServer::CreateMessageI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetGasEstimate", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_STRING, "param01",
                           jsonrpc::JSON_OBJECT, NULL),
        &AbstractZServer::GetGasEstimateI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetTransactionReceipt", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_OBJECT, "param01",
                           jsonrpc::JSON_STRING, NULL),
        &AbstractZServer::GetTransactionReceiptI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetNumPeers", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_INTEGER, NULL),
        &AbstractZServer::GetNumPeersI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetNumTxBlocks", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_STRING, NULL),
        &AbstractZServer::GetNumTxBlocksI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetNumDSBlocks", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_STRING, NULL),
        &AbstractZServer::GetNumDSBlocksI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetNumTransactions", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_STRING, NULL),
        &AbstractZServer::GetNumTransactionsI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetTransactionRate", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_REAL, NULL),
        &AbstractZServer::GetTransactionRateI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetTxBlockRate", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_REAL, NULL),
        &AbstractZServer::GetTxBlockRateI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetDSBlockRate", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_REAL, NULL),
        &AbstractZServer::GetDSBlockRateI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetCurrentMiniEpoch", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_STRING, NULL),
        &AbstractZServer::GetCurrentMiniEpochI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetCurrentDSEpoch", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_STRING, NULL),
        &AbstractZServer::GetCurrentDSEpochI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("DSBlockListing", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_OBJECT, "param01",
                           jsonrpc::JSON_INTEGER, NULL),
        &AbstractZServer::DSBlockListingI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("TxBlockListing", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_OBJECT, "param01",
                           jsonrpc::JSON_INTEGER, NULL),
        &AbstractZServer::TxBlockListingI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetBlockchainInfo", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_OBJECT, NULL),
        &AbstractZServer::GetBlockchainInfoI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetRecentTransactions", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_OBJECT, NULL),
        &AbstractZServer::GetRecentTransactionsI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetShardingStructure", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_OBJECT, NULL),
        &AbstractZServer::GetShardingStructureI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetNumTxnsTxEpoch", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_INTEGER, NULL),
        &AbstractZServer::GetNumTxnsTxEpochI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetNumTxnsDSEpoch", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_STRING, NULL),
        &AbstractZServer::GetNumTxnsDSEpochI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetSmartContractState", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_OBJECT, "param01",
                           jsonrpc::JSON_STRING, NULL),
        &AbstractZServer::GetSmartContractStateI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetSmartContractCode", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_OBJECT, "param01",
                           jsonrpc::JSON_STRING, NULL),
        &AbstractZServer::GetSmartContractCodeI);
    this->bindAndAddMethod(
        jsonrpc::Procedure("GetSmartContractInit", jsonrpc::PARAMS_BY_POSITION,
                           jsonrpc::JSON_OBJECT, "param01",
                           jsonrpc::JSON_STRING, NULL),
        &AbstractZServer::GetSmartContractInitI);
  }

  inline virtual void GetNetworkIdI(const Json::Value& request,
                                    Json::Value& response) {
    (void)request;
    response = this->GetNetworkId();
  }
  inline virtual void CreateTransactionI(const Json::Value& request,
                                         Json::Value& response) {
    response = this->CreateTransaction(request[0u]);
  }
  inline virtual void GetTransactionI(const Json::Value& request,
                                      Json::Value& response) {
    response = this->GetTransaction(request[0u].asString());
  }
  inline virtual void GetDsBlockI(const Json::Value& request,
                                  Json::Value& response) {
    response = this->GetDsBlock(request[0u].asString());
  }
  inline virtual void GetTxBlockI(const Json::Value& request,
                                  Json::Value& response) {
    response = this->GetTxBlock(request[0u].asString());
  }
  inline virtual void GetLatestDsBlockI(const Json::Value& request,
                                        Json::Value& response) {
    (void)request;
    response = this->GetLatestDsBlock();
  }
  inline virtual void GetLatestTxBlockI(const Json::Value& request,
                                        Json::Value& response) {
    (void)request;
    response = this->GetLatestTxBlock();
  }
  inline virtual void GetBalanceI(const Json::Value& request,
                                  Json::Value& response) {
    response = this->GetBalance(request[0u].asString());
  }
  inline virtual void GetMinimumGasPriceI(const Json::Value& request,
                                          Json::Value& response) {
    (void)request;
    response = this->GetMinimumGasPrice();
  }
  inline virtual void GetPrevDSDifficultyI(const Json::Value& request,
                                           Json::Value& response) {
    (void)request;
    response = this->GetPrevDSDifficulty();
  }
  inline virtual void GetPrevDifficultyI(const Json::Value& request,
                                         Json::Value& response) {
    (void)request;
    response = this->GetPrevDifficulty();
  }
  inline virtual void GetSmartContractsI(const Json::Value& request,
                                         Json::Value& response) {
    response = this->GetSmartContracts(request[0u].asString());
  }
  inline virtual void GetContractAddressFromTransactionIDI(
      const Json::Value& request, Json::Value& response) {
    response =
        this->GetContractAddressFromTransactionID(request[0u].asString());
  }
  inline virtual void CreateMessageI(const Json::Value& request,
                                     Json::Value& response) {
    response = this->CreateMessage(request[0u]);
  }
  inline virtual void GetGasEstimateI(const Json::Value& request,
                                      Json::Value& response) {
    response = this->GetGasEstimate(request[0u]);
  }
  inline virtual void GetTransactionReceiptI(const Json::Value& request,
                                             Json::Value& response) {
    response = this->GetTransactionReceipt(request[0u].asString());
  }
  inline virtual void GetNumPeersI(const Json::Value& request,
                                   Json::Value& response) {
    (void)request;
    response = this->GetNumPeers();
  }
  inline virtual void GetNumTxBlocksI(const Json::Value& request,
                                      Json::Value& response) {
    (void)request;
    response = this->GetNumTxBlocks();
  }
  inline virtual void GetNumDSBlocksI(const Json::Value& request,
                                      Json::Value& response) {
    (void)request;
    response = this->GetNumDSBlocks();
  }
  inline virtual void GetNumTransactionsI(const Json::Value& request,
                                          Json::Value& response) {
    (void)request;
    response = this->GetNumTransactions();
  }
  inline virtual void GetTransactionRateI(const Json::Value& request,
                                          Json::Value& response) {
    (void)request;
    response = this->GetTransactionRate();
  }
  inline virtual void GetTxBlockRateI(const Json::Value& request,
                                      Json::Value& response) {
    (void)request;
    response = this->GetTxBlockRate();
  }
  inline virtual void GetDSBlockRateI(const Json::Value& request,
                                      Json::Value& response) {
    (void)request;
    response = this->GetDSBlockRate();
  }
  inline virtual void GetCurrentMiniEpochI(const Json::Value& request,
                                           Json::Value& response) {
    (void)request;
    response = this->GetCurrentMiniEpoch();
  }
  inline virtual void GetCurrentDSEpochI(const Json::Value& request,
                                         Json::Value& response) {
    (void)request;
    response = this->GetCurrentDSEpoch();
  }
  inline virtual void DSBlockListingI(const Json::Value& request,
                                      Json::Value& response) {
    (void)request;
    response = this->DSBlockListing(request[0u].asUInt());
  }
  inline virtual void TxBlockListingI(const Json::Value& request,
                                      Json::Value& response) {
    (void)request;
    response = this->TxBlockListing(request[0u].asUInt());
  }
  inline virtual void GetBlockchainInfoI(const Json::Value& request,
                                         Json::Value& response) {
    (void)request;
    response = this->GetBlockchainInfo();
  }
  inline virtual void GetRecentTransactionsI(const Json::Value& request,
                                             Json::Value& response) {
    (void)request;
    response = this->GetRecentTransactions();
  }
  inline virtual void GetShardingStructureI(const Json::Value& request,
                                            Json::Value& response) {
    (void)request;
    response = this->GetShardingStructure();
  }
  inline virtual void GetNumTxnsTxEpochI(const Json::Value& request,
                                         Json::Value& response) {
    (void)request;
    response = this->GetNumTxnsTxEpoch();
  }
  inline virtual void GetNumTxnsDSEpochI(const Json::Value& request,
                                         Json::Value& response) {
    (void)request;
    response = this->GetNumTxnsDSEpoch();
  }
  inline virtual void GetSmartContractStateI(const Json::Value& request,
                                             Json::Value& response) {
    response = this->GetSmartContractState(request[0u].asString());
  }
  inline virtual void GetSmartContractCodeI(const Json::Value& request,
                                            Json::Value& response) {
    response = this->GetSmartContractCode(request[0u].asString());
  }
  inline virtual void GetSmartContractInitI(const Json::Value& request,
                                            Json::Value& response) {
    response = this->GetSmartContractInit(request[0u].asString());
  }
  virtual std::string GetNetworkId() = 0;
  virtual Json::Value CreateTransaction(const Json::Value& param01) = 0;
  virtual Json::Value GetTransaction(const std::string& param01) = 0;
  virtual Json::Value GetDsBlock(const std::string& param01) = 0;
  virtual Json::Value GetTxBlock(const std::string& param01) = 0;
  virtual Json::Value GetLatestDsBlock() = 0;
  virtual Json::Value GetLatestTxBlock() = 0;
  virtual Json::Value GetBalance(const std::string& param01) = 0;
  virtual std::string GetMinimumGasPrice() = 0;
  virtual Json::Value GetSmartContracts(const std::string& param01) = 0;
  virtual std::string GetContractAddressFromTransactionID(
      const std::string& param01) = 0;
  virtual std::string CreateMessage(const Json::Value& param01) = 0;
  virtual std::string GetGasEstimate(const Json::Value& param01) = 0;
  virtual Json::Value GetTransactionReceipt(const std::string& param01) = 0;
  virtual unsigned int GetNumPeers() = 0;
  virtual std::string GetNumTxBlocks() = 0;
  virtual std::string GetNumDSBlocks() = 0;
  virtual std::string GetNumTransactions() = 0;
  virtual double GetTransactionRate() = 0;
  virtual double GetTxBlockRate() = 0;
  virtual double GetDSBlockRate() = 0;
  virtual uint8_t GetPrevDSDifficulty() = 0;
  virtual uint8_t GetPrevDifficulty() = 0;
  virtual std::string GetCurrentMiniEpoch() = 0;
  virtual std::string GetCurrentDSEpoch() = 0;
  virtual Json::Value DSBlockListing(unsigned int param01) = 0;
  virtual Json::Value TxBlockListing(unsigned int param01) = 0;
  virtual Json::Value GetBlockchainInfo() = 0;
  virtual Json::Value GetRecentTransactions() = 0;
  virtual Json::Value GetShardingStructure() = 0;
  virtual std::string GetNumTxnsDSEpoch() = 0;
  virtual uint32_t GetNumTxnsTxEpoch() = 0;
  virtual Json::Value GetSmartContractState(const std::string& param01) = 0;
  virtual Json::Value GetSmartContractInit(const std::string& param01) = 0;
  virtual Json::Value GetSmartContractCode(const std::string& param01) = 0;
};

class Server : public AbstractZServer {
  Mediator& m_mediator;
  std::pair<uint64_t, boost::multiprecision::uint128_t> m_BlockTxPair;
  std::pair<uint64_t, boost::multiprecision::uint128_t> m_TxBlockCountSumPair;
  uint64_t m_StartTimeTx;
  uint64_t m_StartTimeDs;
  std::pair<uint64_t, CircularArray<std::string>> m_DSBlockCache;
  std::pair<uint64_t, CircularArray<std::string>> m_TxBlockCache;
  static CircularArray<std::string> m_RecentTransactions;
  static std::mutex m_mutexRecentTxns;

 public:
  Server(Mediator& mediator, jsonrpc::HttpServer& httpserver);
  ~Server();

  virtual std::string GetNetworkId();
  virtual Json::Value CreateTransaction(const Json::Value& _json);
  virtual Json::Value GetTransaction(const std::string& transactionHash);
  virtual Json::Value GetDsBlock(const std::string& blockNum);
  virtual Json::Value GetTxBlock(const std::string& blockNum);
  virtual Json::Value GetLatestDsBlock();
  virtual Json::Value GetLatestTxBlock();
  virtual Json::Value GetBalance(const std::string& address);
  virtual std::string GetMinimumGasPrice();
  virtual Json::Value GetSmartContracts(const std::string& address);
  virtual std::string GetContractAddressFromTransactionID(
      const std::string& tranID);
  virtual std::string CreateMessage(const Json::Value& _json);
  virtual std::string GetGasEstimate(const Json::Value& _json);
  virtual Json::Value GetTransactionReceipt(const std::string& transactionHash);
  virtual unsigned int GetNumPeers();
  virtual std::string GetNumTxBlocks();
  virtual std::string GetNumDSBlocks();
  virtual std::string GetNumTransactions();
  virtual double GetTransactionRate();
  virtual double GetTxBlockRate();
  virtual double GetDSBlockRate();
  virtual uint8_t GetPrevDSDifficulty();
  virtual uint8_t GetPrevDifficulty();
  virtual std::string GetCurrentMiniEpoch();
  virtual std::string GetCurrentDSEpoch();
  virtual Json::Value DSBlockListing(unsigned int page);
  virtual Json::Value TxBlockListing(unsigned int page);
  virtual Json::Value GetBlockchainInfo();
  virtual Json::Value GetRecentTransactions();
  virtual Json::Value GetShardingStructure();
  virtual std::string GetNumTxnsDSEpoch();
  virtual uint32_t GetNumTxnsTxEpoch();
  static void AddToRecentTransactions(const dev::h256& txhash);

  // gets the number of transaction starting from block blockNum to most recent
  // block
  size_t GetNumTransactions(uint64_t blockNum);

  Json::Value GetSmartContractState(const std::string& address);
  Json::Value GetSmartContractInit(const std::string& address);
  Json::Value GetSmartContractCode(const std::string& address);
};
