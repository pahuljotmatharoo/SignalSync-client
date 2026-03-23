package services

import (
	"database/sql"
	"fmt"
	"signalsync/backend/database"
)

func ValidateLoginService(db *sql.DB, info *database.Login) (bool, string) {
	rows, errors := db.Query("SELECT password, api_key FROM login WHERE username = ?", info.Username)
	if errors != nil {
		return false, ""
	}
	fmt.Print(errors)
	if !rows.Next() {
		return false, ""
	}
	var password string
	var api_key string
	err := rows.Scan(&password, &api_key)
	fmt.Println(err)
	if err != nil {
		return false, ""
	}
	fmt.Println(password)
	fmt.Println(api_key)
	return password == (info.Password), api_key
}
