import { useRef, useState, useEffect } from "react";
import { getRadarPosition, playerColors } from "../utilities/utilities";

let playerRotations = [];
const calculatePlayerRotation = (playerData) => {
  const playerViewAngle = 270 - playerData.m_eye_angle;
  const idx = playerData.m_idx;

  playerRotations[idx] = (playerRotations[idx] || 0) % 360;
  playerRotations[idx] +=
    ((playerViewAngle - playerRotations[idx] + 540) % 360) - 180;

  return playerRotations[idx];
};

const Player = ({ playerData, mapData, radarImage, localTeam, averageLatency }) => {
  const [lastKnownPosition, setLastKnownPosition] = useState(null);
  const radarPosition = getRadarPosition(mapData, playerData.m_position) || { x: 0, y: 0 };
  const invalidPosition = radarPosition.x <= 0 && radarPosition.y <= 0;

  const playerRef = useRef();
  const playerBounding = (playerRef.current &&
    playerRef.current.getBoundingClientRect()) || { width: 0, height: 0 };
  const playerRotation = calculatePlayerRotation(playerData);

  const radarImageBounding = (radarImage !== undefined &&
    radarImage.getBoundingClientRect()) || { width: 0, height: 0 };

  // Store the last known position when the player dies
  useEffect(() => {
    if (playerData.m_is_dead) {
      if (!lastKnownPosition) {
        console.log(`Storing last known position for player ${playerData.m_idx}:`, radarPosition);
        setLastKnownPosition(radarPosition);
      }
    } else {
      // Reset last known position when the player is alive
      setLastKnownPosition(null);
    }
  }, [playerData.m_is_dead, radarPosition, lastKnownPosition]);

  const effectivePosition = playerData.m_is_dead ? lastKnownPosition || { x: 0, y: 0 } : radarPosition;

  const radarImageTranslation = {
    x: radarImageBounding.width * effectivePosition.x - playerBounding.width * 0.5,
    y: radarImageBounding.height * effectivePosition.y - playerBounding.height * 0.5,
  };

  return (
    <div
      className={`absolute origin-center rounded-[100%] left-0 top-0 w-[1.6vw] h-[1.6vw] lg:w-[0.7vw] lg:h-[0.7vw]`}
      ref={playerRef}
      style={{
        transform: `translate(${radarImageTranslation.x}px, ${radarImageTranslation.y}px) rotate(${(playerData.m_is_dead && `0`) || playerRotation}deg)`,
        transition: `transform ${averageLatency}ms linear`,
        backgroundColor: `${
          (playerData.m_team == localTeam &&
            playerColors[playerData.m_color]) ||
          `red`
        }`,
        opacity: `${
          (playerData.m_is_dead && `0.8`) || (invalidPosition && `0`) || `1`
        }`,
        WebkitMask: `${
          (playerData.m_is_dead &&
            `url('./assets/icons/icon-enemy-death_png.png') no-repeat center / contain`) ||
          `none`
        }`,
        zIndex: `${(playerData.m_is_dead && `0`) || `1`}`,
      }}
    >
      <div
        className={`w-[1.8vw] h-[1.8vw] lg:w-[0.7vw] lg:h-[0.7vw] rotate-[315deg] rounded-[50%_50%_50%_0%]`}
        style={{
          backgroundColor: `${
            (playerData.m_team == localTeam &&
              playerColors[playerData.m_color]) ||
            `red`
          }`,
        }}
      ></div>
 <div
  className="absolute left-1/2 top-1/2 w-[1.5vw] h-[3vw] bg-white opacity-30"
  style={{
    transform: `translate(-50%, 5%) rotate(0deg)`, // Pushes it forward
    clipPath: "polygon(50% 0%, 0% 100%, 100% 100%)", // Triangle pointing forward
  }}
  />
    </div>
  );
};

export default Player;
