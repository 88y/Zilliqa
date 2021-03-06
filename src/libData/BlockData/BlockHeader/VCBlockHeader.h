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

#ifndef __VCBLOCKHEADER_H__
#define __VCBLOCKHEADER_H__

#include <array>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <boost/multiprecision/cpp_int.hpp>
#pragma GCC diagnostic pop
#include <vector>

#include "BlockHeaderBase.h"
#include "common/Constants.h"
#include "common/Serializable.h"
#include "libCrypto/Schnorr.h"
#include "libData/AccountData/Transaction.h"
#include "libNetwork/Peer.h"
#include "libNetwork/PeerStore.h"

/// Stores information on the header part of the VC block.
class VCBlockHeader : public BlockHeaderBase {
  uint64_t m_VieWChangeDSEpochNo;
  uint64_t m_VieWChangeEpochNo;
  unsigned char m_ViewChangeState;
  Peer m_CandidateLeaderNetworkInfo;
  PubKey m_CandidateLeaderPubKey;
  uint32_t m_VCCounter;
  std::vector<std::pair<PubKey, Peer>> m_FaultyLeaders;
  BlockHash m_prevHash;

 public:
  /// Default constructor.
  VCBlockHeader();  // creates a dummy invalid placeholder BlockHeader --
                    // blocknum is maxsize of uint256

  /// Constructor for loading VC block header information from a byte stream.
  VCBlockHeader(const std::vector<unsigned char>& src, unsigned int offset);

  /// Constructor with specified VC block header parameters.
  VCBlockHeader(const uint64_t& vieWChangeDSEpochNo,
                const uint64_t& viewChangeEpochNo,
                const unsigned char viewChangeState,
                const Peer& candidateLeaderNetworkInfo,
                const PubKey& candidateLeaderPubKey, const uint32_t vcCounter,
                const std::vector<std::pair<PubKey, Peer>>& faultyLeaders,
                const CommitteeHash& committeeHash, const BlockHash& prevHash);

  /// Implements the Serialize function inherited from Serializable.
  bool Serialize(std::vector<unsigned char>& dst, unsigned int offset) const;

  /// Implements the Deserialize function inherited from Serializable.
  bool Deserialize(const std::vector<unsigned char>& src, unsigned int offset);

  /// Returns the DS Epoch number where view change happen
  const uint64_t& GetVieWChangeDSEpochNo() const;

  /// Returns the hash of prev dir block
  const BlockHash& GetPrevHash() const { return m_prevHash; }

  /// Returns the Epoch number (Total nums of final block) where view change
  /// happen
  const uint64_t& GetViewChangeEpochNo() const;

  /// Return the candidate leader ds state when view change happen
  unsigned char GetViewChangeState() const;

  /// Return the IP and port of candidate (at the point where view change
  /// happen)
  const Peer& GetCandidateLeaderNetworkInfo() const;

  /// Return pub key of candidate leader
  const PubKey& GetCandidateLeaderPubKey() const;

  /// Return the number of times view change has happened for the particular
  /// epoch and state
  uint32_t GetViewChangeCounter() const;

  /// Return all the faulty leaders in the current round of view change
  const std::vector<std::pair<PubKey, Peer>>& GetFaultyLeaders() const;

  /// Equality operator.
  bool operator==(const VCBlockHeader& header) const;

  /// Less-than comparison operator.
  bool operator<(const VCBlockHeader& header) const;

  /// Greater-than comparison operator.
  bool operator>(const VCBlockHeader& header) const;
};

#endif  // __VCBLOCKHEADER_H__
