package services

import (
	"database/sql"
	"signalsync/backend/database"
)

func ValidateAPISessionService(db *sql.DB, info *database.SessionVerification) bool {
	rows, errors := db.Query("SELECT api_key FROM login WHERE username = ?", info.Username)
	if errors != nil {
		return false
	}
	if !rows.Next() {
		return false
	}
	var api_key string
	err := rows.Scan(&api_key)
	if err != nil {
		return false
	}
	return api_key == (info.Api_key)
}
