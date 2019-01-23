module Auth = { include Auth; };
module People = { include People; };
module Types = { include ApiTypes; };
module YouTube = {
    include YouTube;
    module PlaylistItems = YouTubePlaylistItems;
    module Playlists = YouTubePlaylists;
    module Search = YouTubeSearch;
    module Videos = YouTubeVideos;
    module Types = YouTubeTypes;
};
