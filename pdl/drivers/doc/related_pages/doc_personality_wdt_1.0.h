/**
 *
 * @defgroup PERSONALITY_WDT WDT Personality
 * @ingroup PERSONALITIES
 * @{
 *
 * The Window boundaries configuration:
 * \image html wdt_window.png
 * \image html wdt_window_cfg.png
 *
 * The Pre-Warning Alarm interrupt configuration:
 * \image html wdt_prewarn.png
 * \image html wdt_prewarn_cfg.png
 *
 * \subsection subsection_wdt_codeGen Code Generation
 * The configurator performs the [name]_EventHandler registration in the generated code, so declare it in the application code:
 * \snippet wdt.c snippet_wdt_irqHandlers
 * and then start the block itself (the rest of configurations are performed in the generated code):
 * \snippet wdt.c snippet_wdt_start
 *
 * Refer to the API reference \ref WDT for the API details.
 *
 * Refer to the device Datasheet and Reference Manual for HW details.
 */
